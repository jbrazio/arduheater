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

#include <Arduino.h>
#include "painter.h"

namespace widget {
  namespace middle {
    void draw(const uint8_t& v, const uint8_t& position = 0) {
      // this method should be called at the start of the drawing process
      // it will ensure a consistent font/char init.
      Painter::init();

      const uint8_t br = 2; // border
      const uint8_t sw = Painter::instance()->getWidth();
      const uint8_t sh = Painter::instance()->getHeight();
      const uint8_t fh = Painter::instance()->getFontLineSpacing();
      //const uint8_t fw = Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) s);

      const uint8_t x = 0;
      const uint8_t y = fh + 2 * br + 1;
      const uint8_t w = sw;
      const uint8_t h = sh - ((fh + 2 * br) *2) - 1;

      Painter::instance()->setColorIndex(1);
      Painter::instance()->drawBox(x, y, w, h);

      /*
      Painter::instance()->setColorIndex(0);
      Painter::instance()->drawLine(x, y, w, y);
      Painter::instance()->drawLine(x, y + h, w, y + h);
      */
    }
  };
};

#endif
