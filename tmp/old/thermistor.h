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

#ifndef __THERMISTOR_H__
#define __THERMISTOR_H__

#include "common.h"

#define THERMISTOR_WARMUP_TIME   0
#define THERMISTOR_SLEEP_TIME    0
#define THERMISTOR_REFRESH_TIME  1

class thermistor : public Sensor, public Subject<message_t>
{
public:
  typedef Singleton<thermistor> single;

public:
  thermistor(): Sensor(THERMISTOR_WARMUP_TIME, THERMISTOR_SLEEP_TIME, THERMISTOR_REFRESH_TIME)
  {
    ADCSRA  = bit(ADEN);                               // activate the ADC
    ADCSRA |= bit(ADPS0)  |  bit(ADPS1) | bit(ADPS2);  // with prescaler of 128

    for (uint8_t i = 0; i < array_size(m_cache); i++) { m_cache[i] = 0; }
    m_active_channel = 3; // reset will overflow and scan will start on ch0
    reset();

    bitSet(DIDR0, ADC4D); // disable digital buffer on A4
    bitSet(DIDR0, ADC5D); // disable digital buffer on A5
  }

protected:
  int16_t m_cache[4];
  uint8_t m_active_channel;

protected:
  void update();

public:
  inline float get_temperature(const uint8_t& channel) { return m_cache[channel]; }

public:
  void isr(int16_t reading);
  void reset();

};

#endif
