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

#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <Arduino.h>
#include "enum.h"
#include "struct.h"

class Sensor {
protected:
  Sensor()
    : m_needs_updating(true)
  {;}

public:
  virtual ~Sensor() {;}

public:
  bool m_needs_updating;

protected:
  sensor_state_t m_sensor_state;
  hb_timer_t s_sleep;

protected:
  virtual void update()  {;}

public:
  virtual void init()    {;}
  virtual void timeout() {;}
  virtual void worker()  {;}

public:
  virtual inline void reset() {
    m_sensor_state   = SENSOR_SLEEP;
    s_sleep.timeleft = s_sleep.period;
  }

  virtual inline sensor_state_t state() {
    return m_sensor_state;
  }
};

#endif
