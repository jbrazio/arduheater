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

#ifndef __MACROS_H__
#define __MACROS_H__

#include "arduheater.h"

#ifndef bit
  #define bit(n) (1 << n)
#endif

#ifndef array_size
  #define array_size(a) (sizeof(a) / sizeof(*a))
#endif

#ifndef no_more
  #define no_more(var, high) do { if (var > high) var = high; } while(0)
#endif

#ifndef no_less
  #define no_less(var, low) do { if (var < low) var = low; } while(0)
#endif

#endif
