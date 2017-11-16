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

#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <stdint.h>
#include <stdlib.h>

#include <avr/pgmspace.h>

#include "heater.h"
#include "thermistor.h"

/**
 * @brief TODO
 * @details
 *
 */
class Output
{
  /**
   * Disable the creation of an instance of this object.
   * This class should be used as a static class.
   */
  private:
     Output() {;}
    ~Output() {;}

  protected:
    /**
     * @brief Output channel structure
     * @details Top level output channel structure block.
     *
     */
    static struct channel_t {
      Heater heater;
      Thermistor sensor;

      inline void enable() { heater.start(); }
      inline void disable() { heater.stop(); }
      inline void setpoint(const float &value) { heater.set_target(value); }
    } s_channel[4];

  public:
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static channel_t& channel(const uint8_t& n) { return s_channel[n]; }

  public:
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static void update_sensor(const uint8_t&, const uint16_t&);

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static void update_heater(const uint8_t&, const int16_t&);

};

#endif

