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

#ifndef __UTILS_H__
#define __UTILS_H__

#include "common.h"

namespace utils {
  void itoa(char* buf, uint8_t& pos, int16_t n, const uint8_t& base = 10, uint8_t digits = 0);
  uint8_t mstotick(const uint16_t& ms);
};

namespace utils {
  namespace weather {
    // Uncomment the following line to enable the NOAA dew function
    // otherwise a simpler calculating algorithm will be used.
    //#define NOAA_DEW_FUNCTION

    float dew(const float& t, const float& rh);
  }
};

#endif
