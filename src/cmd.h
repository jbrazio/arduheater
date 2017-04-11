/**
 * Arduheater - Heat controller for astronomy usage
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

#ifndef __CMD_H__
#define __CMD_H__

#include "arduheater.h"

namespace cmd {
  extern void autotune(const char& c);
  extern void buildinfo();
  extern void disableheater(const char& c);
  extern void enableheater(const char& c);
  extern void help();
  extern void process(const char* buffer);
  extern void registers();
  extern void result(const uint8_t& code);
  extern void save();
  extern void set(const uint8_t& key, const float& val);
  extern void settings();
  extern void status();
};

#endif
