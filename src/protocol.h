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

#ifndef __SERIAL_PROTOCOL_H__
#define __SERIAL_PROTOCOL_H__

#include <stdint.h>
#include <stdlib.h>

#include <avr/pgmspace.h>

#include "version.h"
#include "console.h"

#include "analog.h"
#include "output.h"

namespace protocol {
  void autotune(const char&);
  void buildinfo();
  void disableheater(const char&);
  void enableheater(const char&);
  void help();
  void process(const char*);
  void registers();
  void result(const uint8_t&);
  void save();
  void set(const uint8_t&, const float&);
  void settings();
  void status();
};

#endif
