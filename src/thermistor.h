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

#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include <math.h>

#include "macro.h"
#include "type.h"

/**
 * @brief Sensor structure
 * @details Top level sensor structure block.
 *
 */
class Thermistor
{
  public:
    /**
     * @brief Sensor config structure
     * @details This structure stores all the configuration parameters for a thermistor.
     *
     */
    typedef struct {
      float    nominaltemp;
      uint16_t bcoefficient,
               nominalval,
               resistor;
    } config_t;

  protected: // Attributes
    config_t m_config = {
        DEFAULT_NTC_NT, DEFAULT_NTC_BC, DEFAULT_NTC_NV, DEFAULT_NTC_SR
    };

    bool m_state     = false,
         m_connected = false;
    uint16_t m_value = 1023;

  public: // Configuration
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline config_t& config() { return m_config; }

  public: // Methods
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline bool is_connected() {
        return m_connected;
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    float temp();

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline uint16_t raw() { return m_value; }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline void set_connected(const bool &value) { m_connected = value; }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline void set_value(const uint16_t &value) { m_value = value; }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    uint16_t invtemp(const float&);
};

#endif
