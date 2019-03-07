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
  protected:
    environment_config_t m_config;

  public:
    Environment()
    {
      memset(&m_config, 0, sizeof(m_config));
    }

    inline bool is_connected()                                    { return (m_connected);    }
    inline bool     is_ready()                                    { return (m_ready);        }

    inline float temperature(const bool &calibrated = true)       { return (calibrated) ? (m_runtime.temp + m_config.temp_offset) : (m_runtime.temp); }
    inline float    humidity(const bool &calibrated = true)       { return (calibrated) ? (m_runtime.rh   + m_config.rh_offset)   : (m_runtime.rh);   }
    inline float   dew_point()                                    { return (calculate_dew(temperature(), humidity()));                                }

    inline void  set_temp_offset(const float &value)              { m_config.temp_offset = value; }
    inline void    set_rh_offset(const float &value)              { m_config.rh_offset = value;   }

    inline float temp_offset()                                    { return m_config.temp_offset; }
    inline float   rh_offset()                                    { return m_config.rh_offset;   }

    inline environment_config_t& export_config()                  { return m_config;   }
    inline void import_config(const environment_config_t &config) { m_config = config; }
};

extern Environment ambient;

#endif
