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

#include "main.h"

void setup() {
  DDRB |= 0x20; // Enable D13 as output
  Serial.begin(BAUDRATE);
  SERIAL_BANNER;

  ui::single::instance().select_page(PAGE_BOOTSCREEN, 500, PAGE_HOME);

  keypad::single::instance().init(UI_KEYPAD_A_PIN, UI_KEYPAD_B_PIN);
  keypad::single::instance().attach(&ui::single::instance());

  timer1::init();

}

volatile uint32_t test_counter = 0;

void loop() {
  /*
  uint32_t now = millis();
  static unsigned long wait_ui = now + 1000L;

  if ((uint32_t) (now >= wait_ui)) {
    cli();
    wait_ui = now + 1000L;
    serial_print_uint32_base10(test_counter);
    test_counter = 0;
    SERIAL_EOL;
    sei();
  }
  */
}
