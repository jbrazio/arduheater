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

#define REPLY_INVALID_COMMAND   1
#define REPLY_INVALID_SYNTAX    2
#define REPLY_NTC_NOT_READY     3
#define REPLY_OK                4
#define REPLY_OUTPUT_ACTIVE     5
#define REPLY_OUTPUT_INACTIVE   6
#define REPLY_OUTPUT_OUTBOUNDS  7

namespace cmd {
  void autotune(const char& c);
  void buildinfo();
  void disableheater(const char& c);
  void enableheater(const char&c);
  void help();
  void process(const char* buffer);
  void result(const uint8_t& code);
  void status();
};

#endif
