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

#include "common.h"

class CardHome : public Card {
public:
  CardHome() : Card(2, 5000) {;}
  virtual ~CardHome() {;}

public:
  void draw() {
    Painter::instance()->firstPage();
    do {
      char buffer[8];
      uint8_t bufpos = 0;

      widget::title::draw(string_lcd_ambient,  1, true);
      utils::itoa(buffer, bufpos, runtime::single::instance().m_ambient.t());
      buffer[bufpos++] = pgm_read_byte(string_lcd_unit_C);
      buffer[bufpos]   = 0x00;

      widget::middle::draw(buffer, 1);

      bufpos = 0;
      if (m_page_active == 0) {
        widget::title::draw(string_lcd_humidity, 2, true);
        utils::itoa(buffer, bufpos, runtime::single::instance().m_ambient.rh());
        buffer[bufpos++] = pgm_read_byte(string_percent);
        buffer[bufpos]   = 0x00;
      } else {
        widget::title::draw(string_lcd_dew_point, 2, true);
        utils::itoa(buffer, bufpos, runtime::single::instance().m_ambient.dew());
        buffer[bufpos++] = pgm_read_byte(string_lcd_unit_C);
        buffer[bufpos]   = 0x00;
      }

      widget::middle::draw(buffer, 2);

      for (size_t i = 0; i < 4; i++) {
        if (runtime::single::instance().m_output[i].t() != THERMISTOR_ERROR) {
          bufpos = 0;
          utils::itoa(buffer, bufpos, runtime::single::instance().m_output[i].t());
          buffer[bufpos++] = pgm_read_byte(string_lcd_unit_C);
          buffer[bufpos]   = 0x00;
        } else {
          strcpy(buffer, "N/A");
        }

        bool invert = runtime::single::instance().m_output[i].running() ? true : false;
        widget::bottom::draw(buffer, i, false, invert);
      }
    } while(Painter::instance()->nextPage());
  }
};

#endif
