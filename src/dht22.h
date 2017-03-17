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

#ifndef __DHT22_H__
#define __DHT22_H__

#include "arduheater.h"

#define DHT22_WARMUP_TIME   3000L
#define DHT22_SLEEP_TIME    1999L
#define DHT22_REFRESH_TIME  5000L

class dht22: public sensor
{
public:
  dht22();

protected:
  movingmean<float, 10> m_cache[2];

public:
  inline float t()  { return m_cache[0](); }
  inline float rh() { return m_cache[1](); }

public:
  bool hwupdate();
};

#endif
