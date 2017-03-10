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

#ifndef __DHT22_H__
#define __DHT22_H__

#include "common.h"

#define DHT22_WARMUP_TIME   3000L
#define DHT22_SLEEP_TIME    1999L
#define DHT22_REFRESH_TIME  5000L

class DHT22 : public Sensor, public Subject<message_t>
{
public:
  typedef Singleton<DHT22> single;

public:
  DHT22(): Sensor(
    DHT22_WARMUP_TIME,
    DHT22_SLEEP_TIME,
    DHT22_REFRESH_TIME)
  {
    for (size_t i = 0; i < array_size(m_cache); i++) { m_cache[i] = 0; }
  }

protected:
  uint8_t m_pin;
  float m_cache[2];

protected:
  void update();

public:
  inline float get_humidity()       { return m_cache[1]; }
  inline float get_temperature()    { return m_cache[0]; }

public:
  inline void init(const uint8_t& pin) { Sensor::init(); m_pin = pin; }
};

#endif
