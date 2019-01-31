/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2019 João Brázio [joao@brazio.org]
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

#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "dht22.h"

class Environment: public AMBIENT_SENSOR_TYPE {
  public:
    inline bool is_connected()  { return (m_connected);           }
    inline bool is_ready()      { return (m_ready);               }
    inline float temperature()  { return (m_runtime.temp * 0.1F); }
    inline float humidity()     { return (m_runtime.rh   * 0.1F); }
    inline float dew_point()    { return (m_runtime.dew  * 0.1F); }
};

extern Environment ambient;

#endif
