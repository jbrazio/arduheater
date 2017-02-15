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

#ifndef __WIDGET_TITLE_H__
#define __WIDGET_TITLE_H__

/*
 *  How positioning works ?
 *
 *   -----------------
 *  |       -0-       |  0. Top
 *  |-----------------|  5. Bottom
 *  |                 |
 *  |-----------------|
 *  |       -5-       |
 *   -----------------
 *
 *   -------- --------
 *  |   -1-  |   -2-  |  1. Top left
 *  |-------- --------|  2. Top right
 *  |                 |  3. Bottom left
 *  |-------- --------|  4. Bottom right
 *  |   -3-  |   -4-  |
 *   -------- --------
 *
 */

#include <Arduino.h>
#include "painter.h"

namespace widget {
  namespace title {
    void draw(const char* s, const uint8_t& position = 0) {
      // this method should be called at the start of the drawing process
      // it will ensure a consistent font/char init.
      Painter::init();

      const uint8_t br = 2; // border
      const uint8_t sw = Painter::instance()->getWidth();
      const uint8_t sh = Painter::instance()->getHeight();
      const uint8_t fh = Painter::instance()->getFontLineSpacing();
      const uint8_t fw = Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) s);

      uint8_t x, y, w;
      const uint8_t h = fh + 2 * br;

      switch (position) {
        case 0:
          x = 0;
          y = 0;
          w = sw;
          break;

        case 1:
          x = 0;
          y = 0;
          w = sw / 2;
          break;

        case 2:
          x = sw / 2;
          y = 0;
          w = sw / 2;
          break;

        case 3:
          x = 0;
          y = sh - h;
          w = sw / 2;
          break;

        case 4:
          x = sw / 2;
          y = sh - h;
          w = sw / 2;
          break;

        case 5:
          x = 0;
          y = sh - h;
          w = sw;
          break;
      }

      Painter::instance()->setColorIndex(1);
      Painter::instance()->drawBox(x, y, w, h);

      x += (w - fw) / 2;
      Painter::instance()->setColorIndex(0);
      Painter::instance()->drawStrP(x, y + br, (u8g_pgm_uint8_t*) s);
    }
  };
};

#endif
