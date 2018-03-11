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

#ifndef __DHT22_H__
#define __DHT22_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "hal.h"
#include "io.h"
#include "type.h"
#include "utility.h"

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
class DHT22
{
  public:
    volatile typedef struct {
      uint16_t temperature,
               humidity;
      float    dew_point;
      micros_t timestamp;
      uint8_t  errcount;
    } runtime_t;

  protected:
    typedef void (*callback_t)(const float&);
    static callback_t s_callback;

  /**
   * @brief [brief description]
   * @details [long description]
   *
   */
  protected:
    static runtime_t s_runtime;
    static volatile int8_t s_buffer[40];
    static volatile uint8_t s_current_bit;

  /**
   * @brief [brief description]
   * @details [long description]
   *
   */
  public:
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static void setup();

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static void read(const callback_t);

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static inline void isr()
    {
      uint32_t now = micros();
      static uint32_t before = now;

      // When we reach the 40 bits received it's time to deactivate the
      // interrupt routine and process the data.
      if (s_current_bit > (asizeof(s_buffer) +1)) {
        EIMSK &= ~bit(INT0);
        process();

      } else {
        // Filter for data bits
        if (IO::read(AMBIENT_SENSOR_PIN) == LOW) {
          // The DHT22 uses a couple of swings to indicate that it will
          // stream data back to the host, we filter most of this by
          // having the interrupt routine off during the setup of the
          // request but a couple of them are still caught here.
          if (s_current_bit > 1) { s_buffer[(s_current_bit -2)] = now - before; }

          ++s_current_bit;
        }
        before = now;
      }
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static void process();

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    static bool is_fresh();
};

#endif
