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

#ifndef __CARD_LOADING_H__
#define __CARD_LOADING_H__

#include "common.h"

class CardLoading : public Card {
public:
  CardLoading() {;}

public:
  void draw() {
    Painter::instance()->firstPage();
    do {
      Painter::normalfont();
      const uint8_t br = 5; // border
      const uint8_t sw = Painter::instance()->getWidth();
      const uint8_t sh = Painter::instance()->getHeight();
      const uint8_t fh = Painter::instance()->getFontLineSpacing();
      const uint8_t fw = Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_please_wait);

      uint8_t x;
      const uint8_t w = fw + 2 * br;
      const uint8_t h = fh + 2 * br;
      const uint8_t y = sh / 2;

      Painter::instance()->setColorIndex(1);
      Painter::instance()->drawBitmapP(0, 0, 16, 64, bitmap_bootlogo);

      x = (sw - fw) /2 - 2 * br;
      Painter::instance()->setColorIndex(0);
      Painter::instance()->drawBox(x +1, y, w -1, h);

      x = (sw - fw) /2 - br;
      Painter::instance()->setColorIndex(1);
      Painter::instance()->drawStrP(x, y + br, (u8g_pgm_uint8_t*) string_lcd_please_wait);
    } while(Painter::instance()->nextPage());
  }
};

#endif
