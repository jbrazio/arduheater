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
#include "card.h"
#include "print.h"

#include "config.h"

class CardHome : public Card {
public:
  CardHome() {;}
  virtual ~CardHome() {;}

public:
  void draw() {
    int16_t val;
    uint8_t x, y;
    char buffer[4];

    Painter::instance()->firstPage();
    do {
      Painter::instance()->setFont(Painter::get_font());
      Painter::instance()->setFontPosTop();
      Painter::instance()->setFontRefHeightExtendedText();

      const uint8_t border = 2;
      const uint8_t fh = Painter::instance()->getFontLineSpacing() +2;

      Painter::instance()->setColorIndex(1);
      Painter::instance()->drawBox(0, 0, Painter::instance()->getWidth(), fh + 2 * border);

      // Draw the temperature indicator
      y = fh * 0.75;
      Painter::instance()->setScale2x2();
      itoa(roundf(22.0f), buffer, 10);
      x = (Painter::instance()->getWidth() /8.0) - ((Painter::instance()->getStrWidth(buffer)
        + Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_unit_C)) /2.0);
      x += Painter::instance()->drawStr(x, y, buffer);
      x += Painter::instance()->drawStrP(x, y, (u8g_pgm_uint8_t*) string_lcd_unit_C);

      // Draw the humidity indicator
      itoa(roundf(10.0f), buffer, 10);
      x = (Painter::instance()->getWidth() *3 /8.0) - ((Painter::instance()->getStrWidth(buffer)
        + Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) string_percent)) /2.0);
      x += Painter::instance()->drawStr(x, y, buffer);
      x += Painter::instance()->drawStrP(x, y, (u8g_pgm_uint8_t*) string_percent);
      Painter::instance()->undoScale();

      Painter::instance()->setColorIndex(0);
      Painter::instance()->drawBox(Painter::instance()->getWidth() /2.0 - border /2.0, border, border, fh);

      Painter::instance()->setColorIndex(0);
      x = Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_ambient);
      Painter::instance()->drawStrP((Painter::instance()->getWidth() /4.0) - (x /2.0), 2,
        (const u8g_pgm_uint8_t*) string_lcd_ambient);

      x = Painter::instance()->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_humidity);
      Painter::instance()->drawStrP((Painter::instance()->getWidth() *3 /4.0) - (x /2.0), 2,
        (const u8g_pgm_uint8_t*) string_lcd_humidity);
    } while(Painter::instance()->nextPage());
  }
};

#endif
