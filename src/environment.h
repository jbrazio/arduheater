/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2018 João Brázio [joao@brazio.org]
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
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static inline bool is_ready()
    {
      return (s_runtime.temperature == AMBIENT_SENSOR_ERROR_VALUE) ? false : true;
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static inline float get_temperature()
    {
      return s_runtime.temperature;
    };

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static inline float get_humidity()
    {
      return s_runtime.humidity;
    };

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static inline float get_dew_point()
    {
      return calculate_dew(s_runtime.temperature, s_runtime.humidity);
    };
};

#endif
