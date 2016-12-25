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

  serial_print_PGM(PSTR(PROGRAM_NAME)); SERIAL_SPACE;
  serial_print_PGM(PSTR(SHORT_BUILD_VERSION)); SERIAL_SPACE;
  serial_print_PGM(string_serial_start); SERIAL_EOL;

  //KeyPad::single::instance().init();

  timer1_isr_setup();
  /*
  GUI::single::instance().selectPage(PAGE_BOOTSCREEN);
  delay(1000);
  GUI::single::instance().selectPage(PAGE_HOME);
  */
}

void loop() {
  /*
  uint32_t now = millis();
  static unsigned long wait_ui = now + 5000L;

  if ((uint32_t) (now >= wait_ui)) {
    wait_ui = now + 5000L;

    if (GUI::single::instance().selectedPage() == PAGE_HOME) {
      GUI::single::instance().selectItem(
        (GUI::single::instance().selectedItem() == 0) ? 1 : 0
      );
    }
  }
  */
}
