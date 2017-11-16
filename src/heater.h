/**
 * Arduheater - An intelligent dew buster for astronomy
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

#ifndef __HEATER_H__
#define __HEATER_H__

#include <stdint.h>
#include <stdlib.h>

#include "type.h"
#include "config.h"
#include "macro.h"

/**
 * @brief Heater structure
 * @details Top level heater structure block.
 *
 */
class Heater
{
  protected:
    /**
     * @brief Heater config structure
     * @details This structure stores all the configuration parameters for a heater.
     *
     */
    struct config_t {
      bool    autostart = false;
      float   offset    = 0,
              setpoint  = 0;
      uint8_t min       = 0,
              max       = 255;
      float   Kp        = DEFAULT_Kp,
              Ki        = DEFAULT_Ki,
              Kd        = DEFAULT_Kd;
    } m_config;

  public:
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    struct runtime_t {
      int16_t Ierr    = 0,
              lastval = 0;

      // only if debug active ?
      float target = 0;

      int16_t Perr = 0,
              Derr = 0;

      int16_t P = 0,
              D = 0,
              I = 0;

      int16_t u = 0;
    } m_runtime;

  protected:
    bool m_automatic = false, //enum mode i.e. automatic, manual ?
         m_connected = false;
    uint8_t m_value  = 0; // pwm

  public:
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline void set_target(const float &value) { m_config.setpoint = value; }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline uint8_t get_value() { return constrain(m_value, m_config.min, m_config.max); }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline runtime_t& dump_runtime() { return m_runtime; }

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
    inline void start() { m_automatic = (m_connected) ? true : false; }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline void stop() {
      m_automatic       = false;
      m_value           = 0;
      m_runtime.Ierr    = 0;
      m_runtime.lastval = 0;

      m_runtime.target  = 0;
      m_runtime.Perr    = 0;
      m_runtime.Derr    = 0;
      m_runtime.P       = 0;
      m_runtime.D       = 0;
      m_runtime.I       = 0;
      m_runtime.u       = 0;
    }

  public:
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    void eval_pid(const float&);
};
#endif
