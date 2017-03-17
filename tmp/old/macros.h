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

#include "common.h"

#define array_size(a) sizeof(a) / sizeof(*a)

/*
serial::print::string(__PRETTY_FUNCTION__); \
serial::print::chr::space(); \
*/

#ifdef VERBOSE
  #define DEBUGPRN(a, b) do { \
    if (a <= VERBOSE) { serial::println::PGM(PSTR(b)); }} while(0)
#else
  #define DEBUGPRN(a, b) do {} while(0)
#endif

#endif
