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

#ifndef __CARD_HOME_H__
#define __CARD_HOME_H__

#include <Arduino.h>
#include <stdlib.h>
#include "card.h"
#include "config.h"
#include "print.h"
#include "runtime.h"

class CardHome : public Card {
public:
  CardHome() {;}
  virtual ~CardHome() {;}

public:
  void draw() {
    char buffer[4];
    uint8_t x, y, a, b;

    Painter::instance()->firstPage();
    do {
      // this method should be called at the start of the drawing process
      // it will ensure a consistent font/char init.
      Painter::init();

      const uint8_t border = 2;
      const uint8_t fh = Painter::instance()->getFontLineSpacing() +2;

      x = y = 0;
      a = Painter::instance()->getWidth();
      b = fh + 2 * border;
      Painter::instance()->setColorIndex(1);
      Painter::instance()->drawBox(x, y, a, b);

      Painter::instance()->setScale2x2();

      // Draw the temperature indicator ----------------------------------------
      itoa(roundf(runtime::single::instance().m_ambient.t()), buffer, 10);

      // split the screen into 4 equal parts
      x = (Painter::instance()->getWidth() /4.0);

      // calculates the total string width and then find it's middle point
      x -= (Painter::instance()->getStrWidth(buffer)
        + Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_unit_C)) /2.0;

      y = fh * 0.75;

      x += Painter::instance()->drawStr(x, y, buffer);
      x += Painter::instance()->drawStrP(x, y, (u8g_pgm_uint8_t*) string_lcd_unit_C);

      // Draw the humidity indicator -------------------------------------------
      itoa(roundf(runtime::single::instance().m_ambient.rh()), buffer, 10);

      // split the screen into 4 equal parts
      x = (Painter::instance()->getWidth() /4.0);

      // position the text on the last 3/4 of the screen.
      x *= 3;

      // calculates the total string width and then find it's middle point
      x -= (Painter::instance()->getStrWidth(buffer)
        + Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) string_percent)) /2.0;

      x += Painter::instance()->drawStr(x, y, buffer);
      x += Painter::instance()->drawStrP(x, y, (u8g_pgm_uint8_t*) string_percent);

      Painter::instance()->undoScale();

      // Draw the titles -------------------------------------------------------
      Painter::instance()->setColorIndex(0);

      x = (Painter::instance()->getWidth() - border) /2.0;
      y = a = border;
      b = fh;
      Painter::instance()->drawBox(x, y, a, b);

      x = Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_ambient);
      Painter::instance()->drawStrP((Painter::instance()->getWidth() /4.0) - (x /2.0), 2,
        (const u8g_pgm_uint8_t*) string_lcd_ambient);

      x = Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_humidity);
      Painter::instance()->drawStrP((Painter::instance()->getWidth() *3 /4.0) - (x /2.0), 2,
        (const u8g_pgm_uint8_t*) string_lcd_humidity);


      // debug
      Painter::instance()->setColorIndex(1);
      x = Painter::instance()->getWidth() /2.0;
      y = Painter::instance()->getHeight() /2.0;
      Painter::instance()->drawPixel(x, y);

    } while(Painter::instance()->nextPage());
  }
};

#endif
