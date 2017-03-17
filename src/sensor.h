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

#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "arduheater.h"

class sensor
{
protected:
  /*
   * The m_register property is a 8bit "register" with the following flags:
   *
   * bit 0 - SENSOR_NEEDS_WARMUP
   * This flag bit is set when the sensor needs a specifica amount of warmup
   * time before taking the first reading. The required amound of time should
   * be defined using m_timer.warmup.period property.
   *
   * bit 1 - SENSOR_NEEDS_SLEEP
   * This flag is set when the sensor needs a specific amount of rest time
   * after taking each reading, the required amount of time should be defined
   * using m_timer.sleep.period property.
   *
   * bit 2 - SENSOR_NEEDS_REFRESH
   * This flag is set when the sensor needs to be refreshed automatically
   * within a period, the required amound of time should be defined using
   * m_timer.refresh.period property.
   */
  flag_t m_register;

  sensor_state_t m_state;
  bool m_needs_updating;

  struct {
    ticker_t refresh;
    ticker_t sleep;
    ticker_t warmup;
  } m_timer;

public:
  sensor(const uint16_t& warmup,
         const uint16_t& sleep,
         const uint16_t& refresh);

public:
  void init();
  void irq();
  void reset();
  void update();

public:
  virtual bool hwbusy() { return false; }
  virtual bool hwupdate() = 0;
};

#endif
