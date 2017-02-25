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
  CardHome() {;}
  virtual ~CardHome() {;}

public:
  void draw() {
    char buffer[8];

    Painter::instance()->firstPage();
    do {
      widget::title::draw(string_lcd_ambient,  1, true);
      snprintf_P(buffer, sizeof(buffer), PSTR("%i%S"),
        runtime::single::instance().m_ambient.t(), string_lcd_unit_C);
      widget::middle::draw(buffer, 1);

      if (m_page_active == 0) {
        widget::title::draw(string_lcd_humidity, 2, true);
        snprintf_P(buffer, sizeof(buffer), PSTR("%i%S"),
          runtime::single::instance().m_ambient.rh(), string_percent);
      } else {
        widget::title::draw(string_lcd_dew_point, 2, true);
        snprintf_P(buffer, sizeof(buffer), PSTR("%i%S"),
          runtime::single::instance().m_ambient.dew(), string_lcd_unit_C);
      }

      widget::middle::draw(buffer, 2);

      for (size_t i = 0; i < 4; i++) {
        snprintf_P(buffer, sizeof(buffer), PSTR("%iC"),
          runtime::single::instance().m_output[i].t);
        widget::bottom::draw(buffer, i);
      }
    } while(Painter::instance()->nextPage());
  }

  inline void init() { m_pages = 2; }
};

#endif
