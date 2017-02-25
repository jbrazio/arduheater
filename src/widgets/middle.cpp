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

#include "common.h"

void widget::middle::draw(const char* s,
  const uint8_t& position, const bool& pgm) {
  // this method should be called at the start of the drawing process
  // it will ensure a consistent font/char init.
  Painter::normalfont();

  const uint8_t br = 2; // border
  const uint8_t sw = Painter::instance()->getWidth();
  const uint8_t sh = Painter::instance()->getHeight();

  uint8_t fh = Painter::instance()->getFontLineSpacing();
  uint8_t fw = pgm ? Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) s)
                   : Painter::instance()->getStrWidth(s);

  uint8_t x, y, w;
  const uint8_t h = sh - ((fh + 2 * br) *2) - 2;

  switch (position) {
    case 0:
      x = 0;
      w = sw;
      break;

    case 1:
      x = 0;
      w = sw / 2;
      break;

    case 2:
      x = sw / 2;
      w = sw / 2;
      break;
  }

  y = fh + 2 * br + 1;

  Painter::instance()->setColorIndex(1);
  Painter::instance()->drawBox(x, y, w, h);

  Painter::instance()->setColorIndex(0);
  Painter::bigfont();

  fh = Painter::instance()->getFontLineSpacing();
  fw = pgm ? Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) s)
           : Painter::instance()->getStrWidth(s);

  y = (sh - fh) / 2;
  x += (w - fw) / 2;

  if (pgm) { Painter::instance()->drawStrP(x, y, (u8g_pgm_uint8_t*) s); }
  else     { Painter::instance()->drawStr(x, y, s); }
}
