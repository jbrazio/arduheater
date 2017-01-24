/**
 * Arduheater - Telescope heat controller
 * Copyright (C) 2016 João Brázio [joao@brazio.org]
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

#ifndef __UI_H__
#define __UI_H__

#include "../bitmap/bootlogo.h"

class ui : public Observer<message_t>
{
private:
  UI_LCD_TYPE m_painter;
  volatile bool v_outdated;
  volatile menu_page_t v_page;

public:
  typedef Singleton<ui> single;

  ui() __attribute__((optimize ("Os"))) :
    m_painter(UI_LCD_OPTIONS),
    v_outdated(false),
    v_page(PAGE_UNKNOWN)
  {
    m_painter.begin();
    m_painter.setContrast(255);

    #if UI_LCD_ROTATION == 90
      m_painter.setRot90();
    #elif UI_LCD_ROTATION == 180
      m_painter.setRot180();
    #elif UI_LCD_ROTATION == 270
      m_painter.setRot270();
    #endif
  }

  void init() {
    selectPage(PAGE_BLANK);
    draw();
  }

  __attribute__((optimize ("Os")))
  void selectPage(menu_page_t page) {
    v_outdated = true;
    v_page = page;
  }

  __attribute__((always_inline))
  __attribute__((optimize ("O3")))
  inline void worker() {
    if (!v_outdated) { return; }
    v_outdated = false;
  }

  __attribute__((optimize ("O3")))
  void update(const message_t& message) {
    switch(message.type) {
      case MESSAGE_TYPE_UNKNOWN:
        break;

      case MESSAGE_TYPE_BUTTON:
        key_type_t button = static_cast<key_type_t>(message.data.ub[0]);
        key_state_t state = static_cast<key_state_t>(message.data.ub[1]);

        serial_print_PGM(PSTR("buton("));
        serial_print_uint8_base10(button);
        serial_print_PGM(PSTR("): "));
        serial_print_uint8_base10(state);
        SERIAL_EOL;
        break;
    }
  }

  void draw() {
    m_painter.firstPage();

    do {
      switch (v_page) {
        case PAGE_BOOTSCREEN:
          m_painter.setColorIndex(1);
          m_painter.drawBitmapP(0, 0, 16, 64, bitmap_bootlogo);
          break;

        case PAGE_HOME:
          //drawHomescreen();
          break;

        default:
          // clear screen
          break;
      }
    } while(m_painter.nextPage());
  }
};

#endif
