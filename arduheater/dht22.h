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

#ifndef __DHT22_H__
#define __DHT22_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "hal.h"
#include "io.h"
#include "type.h"
#include "utility.h"

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
class DHT22
{
  public:
    typedef struct {
      volatile uint16_t temp, rh, dew;
      volatile uint8_t  error_count, current_bit;
    } runtime_t;

  public:
    typedef void (*callback_t)(const float&);

  protected:
    callback_t m_callback;
    runtime_t m_runtime = { 0, 0, 0, 0, 0 };

    volatile bool m_connected = false;
    volatile bool m_ready = false;

    volatile int8_t m_buffer[40] = { };

  public:
    inline void isr()
    {
      // report the sensor as connected
      m_connected = true;

      uint32_t now = micros();
      static uint32_t before = now;

      // When we reach the 40 bits received it's time to deactivate the
      // interrupt routine and process the data.
      if (m_runtime.current_bit > (asizeof(m_buffer) +1)) {
        EIMSK &= ~bit(INT0);
        process();

      } else {
        // Filter for data bits
        if (IO::read(AMBIENT_SENSOR_PIN) == LOW) {
          // The DHT22 uses a couple of swings to indicate that it will
          // stream data back to the host, we filter most of this by
          // having the interrupt routine off during the setup of the
          // request but a couple of them are still caught here.
          if (m_runtime.current_bit > 1) { m_buffer[(m_runtime.current_bit -2)] = now - before; }
          ++m_runtime.current_bit;
        }

        before = micros();
      }
    }

    inline void setup()
    {
      EICRA &= ~(bit(ISC01) | bit (ISC00)); // clear flags
      EICRA |= bit(ISC00);                  // any edge
      EIMSK |= bit(INT0);                   // enable

      m_runtime.current_bit = 0;
      for (size_t i = 0; i < asizeof(m_buffer); i++) { m_buffer[i] = 0; }
    };

    inline void process()
    {
      int16_t a = 0, h = 0, t = 0;
      for (size_t i = 0; i < asizeof(m_buffer); i++) {
        a <<= 1;
        if (m_buffer[i] > 30) { a |= 1; }

        switch (i) {
          case 15: h = a; break;
          case 31: t = a; a = 0; break;
          default:;
        }
      }

      // checksum validation
      if ((uint8_t) (((uint8_t) h) + (h >> 8) + ((uint8_t) t) + (t >> 8)) != a) {
        ++m_runtime.error_count;
        return;
      }

      else {
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

        m_runtime.error_count = 0;
        m_ready = true;

        // store data
        m_runtime.temp = t;
        m_runtime.rh   = h;
        m_runtime.dew  = (calculate_dew((m_runtime.temp * 0.1F), (m_runtime.rh * 0.1F)) * 10);
      }

      if(m_callback) { m_callback((float)(m_runtime.dew * 0.1F)); }
    }

    inline void read(const callback_t func)
    {
      // Delay 2sec between each reading request
      micros_t now = micros();
      static micros_t before = 0;
      if (now - before <= 2e6) { return; }
      before = now;

      // If a new request comes in and the current_bit is zero
      // it means that the last read failed.
      if (m_runtime.current_bit == 0) { ++m_runtime.error_count; }

      setup();
      m_callback = func;

      // Alerts the user of any consecutive sensor errors
      if (m_runtime.error_count > AMBIENT_SENSOR_ERROR_THRESHOLD) {
        LogLn::PGM(PSTR("W Ambient sensor reading error"));
        m_runtime.error_count = 0;
        m_connected = false;
        m_ready = false;

        // Notify the callback function of the new state of the ambient sensor
        if(m_callback) { m_callback((float)(m_runtime.dew * 0.1F)); }
      }

      IO::set_as_output(AMBIENT_SENSOR_PIN);
      IO::write(AMBIENT_SENSOR_PIN, LOW);

      // Delay 80 000 cycles: 5ms at 16.0 MHz
      // Datasheet spec is 500us min pulse
      asm volatile (
        "    ldi  r18, 104" "\n"
        "    ldi  r19, 229" "\n"
        "1:  dec  r19"  "\n"
        "    brne 1b" "\n"
        "    dec  r18"  "\n"
        "    brne 1b" "\n"
      );

      IO::write(AMBIENT_SENSOR_PIN, HIGH);
      IO::set_as_input(AMBIENT_SENSOR_PIN);
    }
};

#endif
