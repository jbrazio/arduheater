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

#ifndef __GUI_H__
#define __GUI_H__

#include "main.h"
#include "bitmap/bootlogo.h"

class GUI
{
private:
  UI_LCD_TYPE m_painter;
  volatile menu_page_t  v_page      = PAGE_UNKNOWN;
  volatile uint8_t      v_outdated  = true;
  volatile uint32_t     v_keystroke = 0;
  volatile uint8_t      v_item      = 0;

public:
  typedef Singleton<GUI> single;

  __attribute__((optimize ("Os")))
  GUI(): m_painter(UI_LCD_OPTIONS) {
    m_painter.begin();
    m_painter.setContrast(255);

    #if UI_LCD_ROTATION == 90
      m_painter.setRot90();
    #elif UI_LCD_ROTATION == 180
      m_painter.setRot180();
    #elif UI_LCD_ROTATION == 270
      m_painter.setRot270();
    #endif

    selectPage(PAGE_BLANK);
  }

  __attribute__((optimize ("O3")))
  void print(const char& c) {
    m_painter.print(c);
  }

  __attribute__((optimize ("O3")))
  void print(const char* str) {
    while (*str) { print(*str++); }
  }

  __attribute__((optimize ("O3")))
  void print_P(const char* str) {
    char c;
    while ((c = pgm_read_byte(str++))) { print(c); }
  }

  __attribute__((optimize ("O3")))
  void xy(const uint16_t& x, const uint16_t& y) {
    m_painter.setPrintPos(x, y);
  }

  __attribute__((optimize ("Os")))
  void drawBootscreen() {
    m_painter.setColorIndex(1);
    m_painter.drawBitmapP(0, 0, 16, 64, bitmap_bootlogo);
  }

  __attribute__((optimize ("Os")))
  void drawHomescreen() {
    int16_t val;
    char buffer[4];

    m_painter.setColorIndex(1);
    m_painter.drawBox(0, 32, 128, 63);
    m_painter.setFont(u8g_font_profont29r);

    m_painter.setColorIndex(0);

    if (v_item) val = roundf(-10.0f);
    else val = roundf(10.0f);
    itoa(val, buffer, 10);

    xy(1, 58);
    print(buffer);
    print_P(string_lcd_unit_C);

    m_painter.setFont(u8g_font_profont11r);

    xy(65, 46);
    if (v_item) print_P(string_lcd_dew_point);
    else print_P(string_lcd_temperature);

    itoa(roundf(30.0f), buffer, 10);

    xy(65, 58);
    print(buffer);
    print_P(string_percent);
    print_P(string_space);
    print_P(string_lcd_humidity);
  }

  __attribute__((optimize ("Os")))
  bool selectPage(menu_page_t page) {
    v_outdated = true;
    v_page = page;
    return true;
  }

  __attribute__((optimize ("O3")))
  menu_page_t selectedPage() {
    return v_page;
  }

  __attribute__((optimize ("Os")))
  bool selectItem(uint8_t item) {
    v_outdated = true;
    v_item = item;
    return true;
  }

  __attribute__((optimize ("O3")))
  uint8_t selectedItem() {
    return v_item;
  }

  __attribute__((optimize ("Os")))
  bool pageDown() {
    serial_print_PGM(PSTR("pageDown()"));
    SERIAL_EOL;

    v_keystroke = millis();
    selectPage(static_cast<menu_page_t>(v_page - 1));

    if (v_page == 0) {
      v_page = static_cast<menu_page_t>(1);
      v_outdated = false;
      return false;
    }

    serial_print_PGM(PSTR("v_page "));
    serial_print_uint8_base10(v_page);
    SERIAL_EOL;
    return true;
  }

  __attribute__((optimize ("Os")))
  bool pageUp() {
    serial_print_PGM(PSTR("pageUp()"));
    SERIAL_EOL;

    v_keystroke = millis();
    selectPage(static_cast<menu_page_t>(v_page + 1));

    if (v_page == COUNT_menu_page_t) {
      v_page = static_cast<menu_page_t>(v_page - 1);
      v_outdated = false;
      return false;
    }

    serial_print_PGM(PSTR("v_page "));
    serial_print_uint8_base10(v_page);
    SERIAL_EOL;
    return true;
  }

  __attribute__((always_inline))
  __attribute__((optimize ("O3")))
  inline void worker() {
    if (!v_outdated) { return; }
    v_outdated = false;

    m_painter.firstPage();

    do {
      switch (v_page) {
        case PAGE_BOOTSCREEN:
          drawBootscreen();
          break;

        case PAGE_HOME:
          drawHomescreen();
          break;

        default:
          // clear screen
          break;
      }
    } while(m_painter.nextPage());
  }
};

#endif
