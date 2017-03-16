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

#include "common.h"

#define SENSOR_NEEDS_WARMUP   0
#define SENSOR_NEEDS_SLEEP    1
#define SENSOR_NEEDS_REFRESH  2

class Sensor
{
public:
  Sensor(const uint16_t& warmup, const uint16_t& sleep, const uint16_t& refresh) {
    if (warmup) {
      m_register |= bit(SENSOR_NEEDS_WARMUP);
      m_timer.warmup.period = warmup;
      //DEBUGPRN(5, "Sensor: SENSOR_NEEDS_WARMUP flag is set");
    }

    if (sleep) {
      m_register |= bit(SENSOR_NEEDS_SLEEP);
      m_timer.sleep.period = sleep;
      //DEBUGPRN(5, "Sensor: SENSOR_NEEDS_SLEEP flag is set");
    }

    if (refresh) {
      m_register |= bit(SENSOR_NEEDS_REFRESH);
      m_timer.refresh.period = refresh;
      //DEBUGPRN(5, "Sensor: SENSOR_NEEDS_REFRESH flag is set");
    }
  }

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
    hb_timer_t refresh;
    hb_timer_t sleep;
    hb_timer_t warmup;
  } m_timer;

protected:
  virtual void update() = 0;

public:
  virtual void init() {
    if (bit_is_set(m_register, SENSOR_NEEDS_WARMUP)) {
      //DEBUGPRN(5, "Sensor: State changed to SENSOR_WARMUP");
      m_state = SENSOR_WARMUP;
      m_timer.warmup.timeleft = m_timer.warmup.period;
    } else { m_state = SENSOR_READY; }
  }

  virtual inline void reset() {
    if (bit_is_set(m_register, SENSOR_NEEDS_SLEEP)) {
      //DEBUGPRN(5, "Sensor: State changed to SENSOR_SLEEP");
      m_state = SENSOR_SLEEP;
      m_timer.sleep.timeleft = m_timer.sleep.period;
    } else { m_state = SENSOR_READY; }
  }

  virtual void irq() {
    if (m_state == SENSOR_WARMUP) {
      if (m_timer.warmup.timeleft > 0) { m_timer.warmup.timeleft -= HEARTBEAT; }
      else { m_state = SENSOR_READY; /* DEBUGPRN(5, "Sensor: State changed to SENSOR_READY"); */ }
    }

    else if (m_state == SENSOR_READY) {
      if (bit_is_set(m_register, SENSOR_NEEDS_REFRESH)) {
        if (m_timer.refresh.timeleft > 0) { m_timer.refresh.timeleft -= HEARTBEAT; }
        else if (!m_needs_updating) { m_needs_updating = true; }
      }

      if (m_needs_updating) {
        //DEBUGPRN(6, "Sensor: m_needs_updating flag is set");
        if (bit_is_set(m_register, SENSOR_NEEDS_REFRESH)) { m_timer.refresh.timeleft = m_timer.refresh.period; }
        m_needs_updating = false;
        update();
      }
    }

    else if (m_state == SENSOR_SLEEP) {
      if (m_timer.sleep.timeleft > 0) { m_timer.sleep.timeleft -= HEARTBEAT; }
      else { m_state = SENSOR_READY; /* DEBUGPRN(5, "Sensor: State changed to SENSOR_READY"); */ }
    }

    else if (m_state == SENSOR_TIMEOUT || m_state == SENSOR_ERROR) {
      // TODO: We need to handle this in a serious way.. ideas:
      // - critical sensor flag
      // - error threshold when reached major fail is triggerd
      reset();
    }
  }
};

#endif
