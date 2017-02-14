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

#ifndef __MACROS_H__
#define __MACROS_H__

#include <Arduino.h>
#include "print.h"
#include "strings.h"
#include "version.h"

#define array_size(a) sizeof(a) / sizeof(*a)

#ifdef DEBUG
  #define DEBUGPRN(a) serial::println::PGM(PSTR(a))
#else
  #define DEBUGPRN(a) do {} while(0)
#endif


#define SERIAL_BANNER serial::print::PGM(PSTR(PROGRAM_NAME));         \
                      serial::print::chr::space();                    \
                      serial::print::PGM(PSTR(SHORT_BUILD_VERSION));  \
                      serial::print::chr::space();                    \
                      serial::print::PGM(string_serial_start);        \
                      serial::print::chr::eol();

#endif
