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

#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include <U8glib.h>

#include "weatherdata.h"
#include "bitmap/bootlogo.h"

enum ui_screen_t {
  BOOTSCREEN,
  HOMESCREEN,

};

enum ui_button_t {
  UI_BUTTON_A,
  UI_BUTTON_B
};

template <typename T>
class UserInterface {
private:
  T u8g;
  volatile int16_t test;
  const uint32_t timeout = 1500L;
  const uint32_t debounce = 250L;

public:
  UserInterface(): u8g(UI_LCD_OPTIONS) {}

  void setup() {
    this->u8g.begin();
    this->u8g.setColorIndex(1);

    switch(UI_LCD_ROTATION) {
      case 90:
        this->u8g.setRot90();
        break;

      case 180:
        this->u8g.setRot180();
        break;

      case 270:
        this->u8g.setRot270();
        break;
    }

    noInterrupts();
    #ifdef UI_KEYPAD_ENABLE
      PCIFR   = 0;    // clear the PCI Flag Register
      DDRB   |= 0x11; // set PCINT0(D8) and PCINT4(D12) as inputs
      PCICR  |= 0x01; // turn on Pin Change Interrupt PORTB
      PCMSK0 |= 0x11; // set the interrupt pin mask
    #endif
    interrupts();
  }

  void isr() {
    #ifdef UI_KEYPAD_ENABLE
      if (PINB & 0x10) this->button(UI_BUTTON_A);
      else if (PINB & 0x01) this->button(UI_BUTTON_B);
    #endif
  }

  void button(const ui_button_t &button) {
    uint32_t now = millis();
    static uint32_t wait = now;

    if ((uint32_t) (now >= wait)) {
      wait = now + this->debounce;

      switch(button) {
        case UI_BUTTON_A:
          this->test++;
          break;

        case UI_BUTTON_B:
          this->test--;
          break;
      }
    }
  }

  int16_t getTest() {
    return this->test;
  }

  void bootscreen() {
    this->u8g.firstPage();

    do {
      this->u8g.drawBitmapP(0, 0, 16, 64, bitmap_bootlogo);
    } while(this->u8g.nextPage());

    #ifndef DEBUG
      delay(2500L);
    #endif
  }

  void homescreen(weather_data_t *wda) {
    const uint32_t now = millis();
    //const bool frame_1hz = (uint16_t) (now * 0.001f) & 1;

    this->u8g.firstPage();
    do {
      this->drawAmbient(
        (uint16_t) (now * 0.0002f) & 1,
        wda[0]
      );
    } while(this->u8g.nextPage());
  }

  void drawAmbient(const uint8_t &page, weather_data_t &ambient) {
    int16_t val;
    char buffer[64];
    u8g_uint_t w = 0;

    this->u8g.setColorIndex(1);
    this->u8g.drawBox(0, 32, 128, 63);
    this->u8g.setFont(u8g_font_profont29r);

    this->u8g.setColorIndex(0);

    if (page) val = roundf(ambient.dew.get());
    else val = roundf(ambient.temperature.get());

    sprintf_P(buffer, PSTR("%dC"), (int16_t) val);
    w = this->u8g.drawStr(1, 58, buffer);

    this->u8g.setFont(u8g_font_profont11r);

    if (page) this->u8g.drawStrP(w +3, 46, PSTR("Dew point"));
    else this->u8g.drawStrP(w +3, 46, PSTR("Temperature"));

    sprintf_P(buffer, PSTR("%d%% RH"), (int16_t) roundf(ambient.humidity.get()));
    this->u8g.drawStr(w +3, 58, buffer);
  }

  void drawStrip() {}
};

#endif
