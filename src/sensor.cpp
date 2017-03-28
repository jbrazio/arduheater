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

sensor::sensor(const uint16_t& warmup, const uint16_t& sleep, const uint16_t& refresh)
{
  if (warmup) {
    m_register |= SENSOR_NEEDS_WARMUP;
    m_timer.warmup.period = utils::msectotick(warmup);
  }

  if (sleep) {
    m_register |= SENSOR_NEEDS_SLEEP;
    m_timer.sleep.period = utils::msectotick(sleep);
  }

  if (refresh) {
    m_register |= SENSOR_NEEDS_REFRESH;
    m_timer.refresh.period = utils::msectotick(refresh);
  }
}

void sensor::init()
{
  if (m_register & SENSOR_NEEDS_WARMUP) {
    m_state = SENSOR_WARMUP;
    m_timer.warmup.ticks = m_timer.warmup.period;
  } else { m_state = SENSOR_READY; }
}

void sensor::reset()
{
  m_needs_updating = false;
  if (m_register & SENSOR_NEEDS_SLEEP) {
    m_state = SENSOR_SLEEP;
    m_timer.sleep.ticks = m_timer.sleep.period;
  } else {
    m_state = SENSOR_READY;
    if (m_register & SENSOR_NEEDS_REFRESH)
      m_timer.refresh.ticks = m_timer.refresh.period;
  }
}

void sensor::irq()
{
  if (m_state == SENSOR_WARMUP) {
    if (m_timer.warmup.ticks > 0) { --m_timer.warmup.ticks; }
    else { m_state = SENSOR_READY; }
  }

  else if (m_state == SENSOR_READY) {
    if (m_register & SENSOR_NEEDS_REFRESH) {
      if (m_timer.refresh.ticks > 0) { --m_timer.refresh.ticks; }
      else if (!m_needs_updating) { m_needs_updating = true; }
    }

    if (m_needs_updating) {
      // we need to address two different types of sensor hardware updates: sync and async
      // if the sensor hardware is sync the return value of hwupdate() should be true and
      // the process is terminated right now, otherwise the sensor will change it's state
      // to BUSY and we will process it later on.
      if (hwupdate()) { reset(); }
    }
  }

  else if (m_state == SENSOR_BUSY) {
    if (m_needs_updating) {
      // here is were the async sensor hardware is processed, we query the sensor status using
      // the hwbusy() method, it shall return true when the data is ready. We'll keep pooling
      // the sensor if the return value is false.
      if (hwbusy()) { reset(); }
    }
  }

  else if (m_state == SENSOR_SLEEP) {
    if (m_timer.sleep.ticks > 0) { --m_timer.sleep.ticks; }
    else {
      m_state = SENSOR_READY;
      if (m_register & SENSOR_NEEDS_REFRESH)
        m_timer.refresh.ticks = m_timer.refresh.period;
    }
  }

  else if (m_state == SENSOR_TIMEOUT || m_state == SENSOR_ERROR) {
    // TODO: We need to handle this in a serious way.. ideas:
    // - critical sensor flag
    // - error threshold when reached major fail is triggerd
    reset();
  }
}
