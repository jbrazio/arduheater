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

#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include <stdio.h>

#include "macro.h"

class Analog
{
  /**
   * Disable the creation of an instance of this object.
   * This class should be used as a static class.
   */
  private:
     Analog() {;}
    ~Analog() {;}

  public:
    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    typedef void (*callback_t)(const uint8_t&, const uint16_t&);

  private:
    typedef struct
    {
      volatile uint8_t  n;
      volatile uint8_t  chan;
      volatile uint16_t raw[255];
    } buffer_t;

    static buffer_t s_buffer;
    static callback_t s_callback;

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
    static void read(const uint8_t&, const callback_t);

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static void set_callback(const callback_t callback)
    {
      s_callback = callback;
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static void callback(const uint8_t& channel, const uint16_t& value)
    {
      if(s_callback) { s_callback(channel, value); }
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static void set_channel(const uint8_t& channel)
    {
      s_buffer.chan = channel;
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static uint8_t get_channel()
    {
      return s_buffer.chan;
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static void clear_buffer()
    {
      s_buffer.n = 0;

      for(uint8_t i = 0; i < buffer_size(); i++) {
        s_buffer.raw[i] = 0;
      }
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static void add_to_buffer(const uint16_t& value)
    {
      s_buffer.raw[s_buffer.n++] = value;
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static uint16_t get_from_buffer(const uint8_t& index)
    {
      return s_buffer.raw[index];
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static uint8_t buffer_size()
    {
      return asizeof(s_buffer.raw);
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static bool is_buffer_full()
    {
      return (s_buffer.n == asizeof(s_buffer.raw));
    }

    /**
     * @brief [brief description]
     * @details [long description]
     *
     */
    inline static void* get_buffer_ptr()
    {
      return (void *) &s_buffer.raw;
    }
};

#endif
