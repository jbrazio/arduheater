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

#include "common.h"

struct message_t {
  msg_category_t category;
  union {
    float        f; // float
    int32_t     dw; // double word
    uint32_t   udw; // unsigned double word
    int16_t   w[2]; // word
    uint16_t uw[2]; // unsigned word
    int8_t    b[4]; // byte
    uint8_t  ub[4]; // unsigned byte
  } data[2];
};

struct key_t {
  int8_t     pin;
  keypress_t state;
  uint8_t    timeout;
};

/*
 * Heartbeat timer
 * TODO: We need a better name for this
 */
struct hb_timer_t {
  // uint16_t will allow up to ~65s periods but due to the way we do time
  // accounting the max value is capped to ~32s.
  uint16_t period;
  int16_t  timeleft;
};

#endif
