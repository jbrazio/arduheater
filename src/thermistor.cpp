/**
 * Arduheater - Telescope heat controller
 * Copyright (C) 2016-2017 João Brázio [joao@brazio.org]
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

#include "common.h"

void thermistor::init() {
  DEBUGPRN("thermistor::init()");

  ADCSRA  = bit(ADEN);                              // Activate the ADC
  ADCSRA |= bit(ADPS0) |  bit(ADPS1) | bit(ADPS2);  // Prescaler of 128

  for (uint8_t i = 0; i < array_size(m_cache); i++) { m_cache[i] = 0; }
  m_active_channel = 3;
  reset();

  bitSet(DIDR0, ADC4D);  // disable digital buffer on A4
  bitSet(DIDR0, ADC5D);  // disable digital buffer on A5
}

void thermistor::update() {
  m_sensor_state = SENSOR_BUSY;
  ADCSRA |= bit(ADSC) | bit(ADIE);
}

void thermistor::irq() {
  switch (m_sensor_state) {
    case SENSOR_READY: {
      if (m_needs_updating) {
        m_needs_updating = false;
        update();
      }
      break;
    }
    default: {
      break;
    }
  }
}

void thermistor::isr(int16_t reading) {
  float steinhart;
  steinhart = SERIESRESISTOR / (1023.0 / reading - 1);  // convert raw to ohms
  steinhart = steinhart / THERMISTORNOMINAL;            // (R/Ro)
  steinhart = log(steinhart);                           // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                            // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);     // + (1/To)
  steinhart = 1.0 / steinhart;                          // Invert
  steinhart -= 273.15;                                  // convert to K to C

  m_cache[m_active_channel] = steinhart;

  //serial::print::PGM(PSTR("t: "));
  //serial::println::float32(steinhart, 2);

  message_t payload;
  payload.category      = MSG_CAT_NTC;
  payload.data[0].ub[0] = m_active_channel;
  payload.data[1].f     = m_cache[m_active_channel];
  notify(payload);

  reset();
}

void thermistor::reset() {
  m_needs_updating = true;
  m_sensor_state   = SENSOR_READY;
  m_active_channel = (m_active_channel +1) % 4;

  // Select internal reference of 1.1V and input port
  ADMUX = bit (REFS0) | bit (REFS1) | (m_active_channel & 0x07);
}
