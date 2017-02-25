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

class thermistor : public Sensor
  , public Subject<message_t>
{
protected:
  int16_t m_cache[4];
  uint8_t m_active_channel;

public:
  typedef Singleton<thermistor> single;

protected:
  void update();

public:
  void init();
  void irq();
  void isr(int16_t reading);
  void reset();

public:
  inline float get_temperature(const uint8_t& channel) { return m_cache[channel]; }
};

#endif
