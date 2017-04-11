/**
 * Arduheater - Heat controller for astronomy usage
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

#include "arduheater.h"

// TODO: Build a thermal protection system

thermistor::thermistor()
  : sensor(THERMISTOR_WARMUP_TIME, THERMISTOR_SLEEP_TIME, THERMISTOR_REFRESH_TIME)
{
  m_active_channel = 0;
}

bool thermistor::hwupdate()
{
  m_state = SENSOR_BUSY;                          // mark the hw as busy
  adc::selchan((const uint8_t) m_active_channel); // select the adc m_active_channel
  adc::update();                                  // trigger an adc update
  return false;                                   // waiting for the adc to get a reading
}

bool thermistor::hwbusy()
{
  if (m_state == SENSOR_BUSY) {
    if (sys.state & ADC_READING_DONE) {
      if (adc::runtime.value > THERMISTOR_MIN_VAL || adc::runtime.value < THERMISTOR_MAX_VAL) {
        m_cache[m_active_channel] = THERMISTOR_ERR_TEMP;


        if (is_ready(m_active_channel)) {
          mark_as_not_ready(m_active_channel);

          // only print disconnect messages if system is running
          if (sys.state & RUNNING) {
            serial::print::PGM(PSTR("warn: out"));
            serial::print::uint8(m_active_channel);
            serial::println::PGM(PSTR(" sensor disconnected"));
          }
        }

      } else {
        m_cache[m_active_channel] += (uint16_t) adc::runtime.value;

        if (! is_ready(m_active_channel)) {
          mark_as_ready(m_active_channel);

          // only print connect messages if system is running
          if (sys.state & RUNNING) {
            serial::print::PGM(PSTR("warn: out"));
            serial::print::uint8(m_active_channel);
            serial::println::PGM(PSTR(" sensor connected"));
          }
        }
      }

      m_active_channel = (m_active_channel +1) % NUM_OUTPUTS;
      return true; // the hw update is complete
    }
  }
  return false; // the hw update is not complete
}
