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
  CardHome() {
    set_slideshow_pages(2);
    set_slideshow_period(5000);
    set_ui_timeout(5000);

    m_active_section = -1;
    m_section_selected = false;
  }

protected:
  int8_t m_active_section;
  bool m_section_selected;
  int8_t m_offset[4];

public:
  void draw() {
    Painter::instance()->firstPage();
    do {
      char buffer[8];
      uint8_t bufpos = 0;

      widget::title::draw(string_lcd_ambient,  1, true);
      utils::itoa(buffer, bufpos, runtime::single::instance().ambient.t());
      buffer[bufpos++] = pgm_read_byte(string_lcd_unit_C);
      buffer[bufpos]   = 0x00;

      widget::middle::draw(buffer, 1);

      bufpos = 0;
      if (m_active_page == 0) {
        widget::title::draw(string_lcd_humidity, 2, true);
        utils::itoa(buffer, bufpos, runtime::single::instance().ambient.rh());
        buffer[bufpos++] = pgm_read_byte(string_percent);
        buffer[bufpos]   = 0x00;
      } else {
        widget::title::draw(string_lcd_dew_point, 2, true);
        utils::itoa(buffer, bufpos, runtime::single::instance().ambient.dew());
        buffer[bufpos++] = pgm_read_byte(string_lcd_unit_C);
        buffer[bufpos]   = 0x00;
      }

      widget::middle::draw(buffer, 2);

      for (size_t i = 0; i < 4; i++) {
        if (runtime::single::instance().heater[i].t() != THERMISTOR_ERR_TEMP) {
          bufpos = 0;

          if (m_section_selected && m_active_section == (int8_t) i) {
            utils::itoa(buffer, bufpos, m_offset[i]);
          } else {
            bufpos = 0;
            utils::itoa(buffer, bufpos, runtime::single::instance().heater[i].t());
            buffer[bufpos++] = pgm_read_byte(string_lcd_unit_C);
            buffer[bufpos]   = 0x00;
          }
        } else { strcpy(buffer, "N/A"); }

        widget::bottom::draw(buffer, i, false, (m_active_section == (int8_t) i));
      }
    } while(Painter::instance()->nextPage());
  }

  inline void left(const bool& extended) {
    switch(extended) {
      case true: {
        m_section_selected = true;
        break;
      }

      case false : {
        switch(m_section_selected) {
          case true: {
            --m_offset[m_active_section];
            break;
          }

          case false: {
            --m_active_section;
            if (m_active_section < 0) { m_active_section = 3; }
            break;
          }
        }
      }
    }
  }

  inline void right(const bool& extended) {
    switch(extended) {
      case true: {
        m_section_selected = true;
        break;
      }

      case false : {
        switch(m_section_selected) {
          case true: {
            ++m_offset[m_active_section];
            break;
          }

          case false: {
            m_active_section = (m_active_section +1) % 4;
            break;
          }
        }
      }
    }
  }

  inline void timeout() {
    m_active_section = -1;
    m_section_selected = false;
  }
};

#endif
