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

#include <Arduino.h>
#include <U8glib.h>
#include "observer.h"
#include "struct.h"
#include "singleton.h"
#include "config.h"
#include "runtime.h"
#include "../bitmap/bootlogo.h"

class ui : public Observer<message_t>
{
private:
  UI_LCD_TYPE m_painter;

public:
  typedef Singleton<ui> single;

  ui() : m_painter(UI_LCD_OPTIONS) {
    m_painter.begin();
    m_painter.setContrast(255);

    #if UI_LCD_ROTATION == 90
      m_painter.setRot90();
    #elif UI_LCD_ROTATION == 180
      m_painter.setRot180();
    #elif UI_LCD_ROTATION == 270
      m_painter.setRot270();
    #endif

    select_page(PAGE_BLANK);
  }

  void print(const char& c) {
    m_painter.print(c);
  }

  void print(const char* str) {
    while (*str) { print(*str++); }
  }

  void print_P(const char* str) {
    char c;
    while ((c = pgm_read_byte(str++))) { print(c); }
  }

  void xy(const uint16_t& x, const uint16_t& y) {
    m_painter.setPrintPos(x, y);
  }

  void font(const menu_font_t& size) {
    const u8g_fntpgm_uint8_t* font = (size == FONT_BIG)
      ? u8g_font_profont29r : u8g_font_profont11r;

    m_painter.setFont(font);
    m_painter.setFontRefHeightExtendedText();
    m_painter.setFontPosTop();
  }

  void select_page(const menu_page_t& page,
  const uint16_t& timeout = 0, const menu_page_t& next = PAGE_UNKNOWN) {
    runtime::single::instance().lcd.page.v_timeout = timeout;

    if (timeout > 0) {
      runtime::single::instance().lcd.page.v_timeout = timeout / HEARTBEAT;
      runtime::single::instance().lcd.page.v_next = (next != PAGE_UNKNOWN)
        ? next : runtime::single::instance().lcd.page.v_current;
    }

    runtime::single::instance().lcd.page.v_current = page;
    runtime::single::instance().lcd.page.v_outdated = true;
  }

  void update(const message_t& message) {
    switch(message.type) {
      case MESSAGE_TYPE_UNKNOWN:
        break;

      case MESSAGE_TYPE_BUTTON:
        key_type_t button = static_cast<key_type_t>(message.data.ub[0]);
        keypress_t state  = static_cast<keypress_t>(message.data.ub[1]);

        serial::print::PGM(PSTR("buton("));
        serial::print::uint8(button);
        serial::print::PGM(PSTR("): "));
        serial::print::uint8(state);
        serial::print::chr::eol();

        if (state == KEYPRESS_SHORT) select_page(PAGE_TEST_A, 1000);
        else select_page(PAGE_TEST_B, 5000, PAGE_HOME);
        break;
    }
  }

  __attribute__((always_inline))
  inline void worker() {
    if (runtime::single::instance().lcd.page.v_timeout
    && ! runtime::single::instance().lcd.page.v_outdated) {
      --runtime::single::instance().lcd.page.v_timeout;
      if (runtime::single::instance().lcd.page.v_timeout == 0)
        select_page((menu_page_t) runtime::single::instance().lcd.page.v_next);
    }

    if (!runtime::single::instance().lcd.page.v_outdated) { return; }
    runtime::single::instance().lcd.page.v_outdated = false;

    m_painter.firstPage();
    do {
      switch (runtime::single::instance().lcd.page.v_current) {
        case PAGE_BOOTSCREEN:
          m_painter.setColorIndex(1);
          m_painter.drawBitmapP(0, 0, 16, 64, bitmap_bootlogo);
          break;

        case PAGE_HOME:
          draw_page_home();
          break;

        case PAGE_TEST_A:
          draw_test_page(1);
          break;

        case PAGE_TEST_B:
          draw_test_page(2);
          break;

        default:
          // clear screen
          break;
      }
    } while(m_painter.nextPage());
  }

  void draw_page_home() {
    int16_t val;
    char buffer[4];

    m_painter.setColorIndex(1);
    m_painter.drawBox(0, 32, 128, 63);

    m_painter.setColorIndex(0);
    font(FONT_BIG);

    val = roundf(10.0f);
    itoa(val, buffer, 10);

    xy(5, 35);
    print(buffer);
    print_P(string_lcd_unit_C);

    font(FONT_SMALL);

    xy(65, 35);
    print_P(string_lcd_temperature);

    itoa(roundf(30.0f), buffer, 10);

    xy(65, 45);
    print(buffer);
    print_P(string_percent);
    print_P(string_space);
    print_P(string_lcd_humidity);
  }

  void draw_test_page(const uint8_t& t) {
    m_painter.setFont(u8g_font_profont29r);
    m_painter.setFontRefHeightExtendedText();
    m_painter.setFontPosTop();

    m_painter.setColorIndex(1);

    uint8_t h = m_painter.getFontAscent() - m_painter.getFontDescent();
    uint8_t w = m_painter.getStrWidth("(AgI)");
    m_painter.drawStr(3, 10, "(AgI)");
    m_painter.drawFrame(3-1, 10, w+2, h+2) ;
  }
};

#endif
