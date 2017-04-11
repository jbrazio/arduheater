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

#ifndef __THERMISTOR_H__
#define __THERMISTOR_H__

#include "arduheater.h"

extern volatile system_t sys;

#define THERMISTOR_WARMUP_TIME   500L
#define THERMISTOR_SLEEP_TIME    0L
#define THERMISTOR_REFRESH_TIME  1L

class thermistor: public sensor
{
public:
  thermistor();

protected:
  movingmean<int16_t, 10> m_cache[NUM_OUTPUTS];
  uint8_t m_active_channel;

public:
  inline float t(const uint8_t& channel) {
    if (channel > NUM_OUTPUTS) return utils::steinhart(THERMISTOR_ERR_TEMP);
    else return utils::steinhart(m_cache[channel]());
  }

  inline float raw(const uint8_t& channel) {
    if (channel > NUM_OUTPUTS) return THERMISTOR_ERR_TEMP;
    else return m_cache[channel]();
  }

  inline bool is_ready(const uint8_t& channel) {
    if (channel > NUM_OUTPUTS) return false;
    return (sys.sensor & (NTC0_SENSOR_READY << channel));
  }

  inline void mark_as_ready(const uint8_t& channel) {
    if (channel > NUM_OUTPUTS) return;
    sys.sensor |= (NTC0_SENSOR_READY << channel);
  }

  inline void mark_as_not_ready(const uint8_t& channel) {
    if (channel > NUM_OUTPUTS) return;
    sys.sensor &= ~(NTC0_SENSOR_READY << channel);
    m_cache[channel] = THERMISTOR_ERR_TEMP;
  }

public:
  bool hwbusy();
  bool hwupdate();
};

#endif
