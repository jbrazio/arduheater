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

#ifndef __HEATER_H__
#define __HEATER_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "macro.h"
#include "type.h"

/**
 * @brief Heater structure
 * @details Top level heater structure block.
 *
 */
class Heater
{
  public:
    /**
     * @brief Heater config structure
     * @details This structure stores all the configuration parameters for a heater.
     *
     */
    typedef struct {
      bool    autostart;
      float   offset,
              setpoint;
      uint8_t min,
              max;
      float   Kp,
              Ki,
              Kd;
    } config_t;

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    typedef struct {
      int32_t Ierr;
      int16_t Derr;

      // only if debug active ?
      float target;

      int16_t Perr;

      int16_t P,
              D,
              I;

      int16_t u;
    } runtime_t;

  protected:
    config_t m_config = {
      false, 0, 0, 0, 255, DEFAULT_Kp, DEFAULT_Ki, DEFAULT_Kd
    };

    runtime_t m_runtime = {
      0, 0, 0, 0, 0, 0, 0, 0
    };

    bool m_automatic = false, //enum mode i.e. automatic, manual ?
         m_connected = false;
    uint8_t m_value  = 0; // pwm

  public:
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
    inline void set_target(const float &value) { m_config.setpoint = value; }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline uint8_t get_value() { return m_value; }

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
      m_runtime.Derr    = 0;

      // only if debug active ?
      m_runtime.target  = 0;
      m_runtime.Perr    = 0;

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
