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

#ifndef __WIDGET_MIDDLE_H__
#define __WIDGET_MIDDLE_H__

#include "common.h"

namespace widget {
  namespace title {
    void draw(const char* s,
      const uint8_t& position = 0, const bool& pgm = false);
  }

  namespace middle {
    void draw(const char* s,
      const uint8_t& position = 0, const bool& pgm = false);
  }

  namespace bottom {
    void draw(const char* s, const uint8_t& position = 0,
      const bool& pgm = false, const bool& invert = false, const bool& blink = false);
  }
};

#endif
