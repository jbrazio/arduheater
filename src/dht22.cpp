/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2018 João Brázio [joao@brazio.org]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "dht22.h"

/**
 * @brief Global variable initialization
 */
volatile int8_t  DHT22::s_buffer[40] = {};
volatile uint8_t DHT22::s_current_bit = 0;

volatile DHT22::runtime_t DHT22::s_runtime = {
  AMBIENT_SENSOR_ERROR_VALUE, AMBIENT_SENSOR_ERROR_VALUE, AMBIENT_SENSOR_ERROR_VALUE, 0, 0
};

DHT22::callback_t DHT22::s_callback = NULL;

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void DHT22::setup() {
  EICRA &= ~(bit(ISC01) | bit (ISC00)); // clear flags
  EICRA |= bit(ISC00);                  // any edge
  EIMSK |= bit(INT0);                   // enable

  s_current_bit = 0;
  s_runtime.timestamp = micros();
  for (size_t i = 0; i < asizeof(s_buffer); i++) { s_buffer[i] = 0; }
};

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void DHT22::read(const callback_t func)
{
  // Delay 2sec between each reading request
  if (micros() - s_runtime.timestamp <= 2e6) { return; }

  // If a new request comes in and the current_bit is zero
  // it means that the last read failed.
  if (s_current_bit == 0) { ++s_runtime.errcount; }

  // Alerts the user of any consecutive sensor errors
  if (s_runtime.errcount >= AMBIENT_SENSOR_ERROR_THRESHOLD) {
    LogLn::PGM(PSTR("WARN: Ambient sensor reading error"));
    s_runtime.errcount = 0; // Be quiet for a while now
  }

  setup();
  DHT22::s_callback = func;

  IO::set_as_output(AMBIENT_SENSOR_PIN);
  IO::write(AMBIENT_SENSOR_PIN, LOW);

  // Delay 16 000 cycles: 1ms at 16 MHz
  // Datasheet spec is 500us min pulse
  asm volatile (
    "    ldi  r18, 21"  "\n"
    "    ldi  r19, 199" "\n"
    "1:  dec  r19"      "\n"
    "    brne 1b"       "\n"
    "    dec  r18"      "\n"
    "    brne 1b"       "\n"
  );

  IO::write(AMBIENT_SENSOR_PIN, HIGH);
  IO::set_as_input(AMBIENT_SENSOR_PIN);
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void DHT22::process() {
  int16_t a = 0, h = 0, t = 0;
  for (size_t i = 0; i < asizeof(s_buffer); i++) {
    a <<= 1;
    if (s_buffer[i] > 30) { a |= 1; }

    switch (i) {
      case 15: h = a; break;
      case 31: t = a; a = 0; break;
      default:;
    }
  }

  // checksum validation
  if ((uint8_t) (((uint8_t) h) + (h >> 8) + ((uint8_t) t) + (t >> 8)) != a) {
    ++s_runtime.errcount;
    return;
  }

  // negative value
  if (t & bit(15)) { t = -((int16_t) (t & 0x7FFF)); }

  static int16_t prev_h = h,
                 prev_t = t;

  // calculate the lpw current output
  h = h + AMBIENT_SENSOR_H_LPF_ALPHA * (prev_h - h);
  prev_h = h;

  // calculate the lpw current output
  t = t + AMBIENT_SENSOR_T_LPF_ALPHA * (prev_t - t);
  prev_t = t;

  CRITICAL_SECTION_START
  s_runtime.errcount    = 0;
  s_runtime.humidity    = (int16_t)(h * 0.1);
  s_runtime.temperature = (int16_t)(t * 0.1);
  s_runtime.dew_point   = calculate_dew(s_runtime.temperature, s_runtime.humidity);
  CRITICAL_SECTION_END

  DHT22::s_callback((float)s_runtime.dew_point);
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
bool DHT22::is_fresh()
{
  return (micros() - s_runtime.timestamp < 6e7) ? true : false;
}
