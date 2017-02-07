/**
 * Arduheater - Telescope heat controller
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

#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <Arduino.h>
#include "enum.h"

struct message_t {
  message_type_t type;
  union {
    float    f;
    int32_t  dw;
    int16_t  w[2];
    int8_t   b[4];
    uint32_t udw;
    uint16_t uw[2];
    uint8_t  ub[4];
  } data;
};

struct key_t {
  int8_t     pin;
  keypress_t state;
  uint8_t    timeout;
};

#endif
