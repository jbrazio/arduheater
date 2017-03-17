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

/*
 *  How positioning works ?
 *
 *   -------------------
 *  |                   |  0. 1/4 bottom
 *  |-------------------|  1. 2/4 bottom
 *  |                   |  2. 3/4 bottom
 *  |-------------------|  3. 4/4 bottom
 *  | 0  | 1  | 2  | 3  |
 *   -------------------
 *
 */

void widget::bottom::draw(const char* s,
  const uint8_t& position, const bool& pgm, const bool& invert) {
  // this method should be called at the start of the drawing process
  // it will ensure a consistent font/char init.
  Painter::normalfont();

  const uint8_t br = 2; // border
  const uint8_t sw = Painter::instance()->getWidth();
  const uint8_t sh = Painter::instance()->getHeight();
  const uint8_t fh = Painter::instance()->getFontLineSpacing();
  const uint8_t fw = pgm ? Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) s)
                         : Painter::instance()->getStrWidth(s);

  uint8_t x;
  const uint8_t w = sw / 4;
  const uint8_t h = fh + 2 * br;
  const uint8_t y = sh - h;

  x = w * position;
  Painter::instance()->setColorIndex(invert ? 1 : 0);
  Painter::instance()->drawBox(x +1, y, w -1, h);

  x += (w - fw) / 2;

  Painter::instance()->setColorIndex(invert ? 0 : 1);

  if (pgm) { Painter::instance()->drawStrP(x, y + br, (u8g_pgm_uint8_t*) s); }
  else     { Painter::instance()->drawStr(x, y + br, s); }
}
