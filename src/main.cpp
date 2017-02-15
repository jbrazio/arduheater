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

#include <Arduino.h>
#include "config.h"
#include "enum.h"
#include "keypad.h"
#include "main.h"
#include "ui.h"

#include "dht22.h"
#include "runtime.h"
#include "circularqueue.h"
#include "serial.h"

void setup() {
  DDRB |= 0x20; // Enable D13 as output

  serial::init();

  /*
  serial::print::banner();
  for (size_t j = 0; j < 100; j++)
    for (size_t i = 0; i < 10; i++)
      serial::print::banner();

  PORTB |= 0x20;
  while (true) {;}
*/







  serial::print::banner();

  ui::single::instance().show(CARD_SPLASH, 750L);

/*
  CircularQueue<uint8_t, 32> foo;
  for (size_t i = 0; i < 128; i++) { foo.enqueue(i); }
  while (foo.count() > 0) { serial::println::uint8(foo.dequeue()); }
*/

  keypad::single::instance().init(UI_KEYPAD_A_PIN, UI_KEYPAD_B_PIN);
  keypad::single::instance().attach(&ui::single::instance());
  DHT22::single::instance().init(AMBIENT_PIN);
  DHT22::single::instance().attach(&runtime::single::instance());
  timer1::init();
}

volatile uint32_t test_counter = 0;

void loop() {
  //++test_counter;
  uint32_t now = millis();
  static unsigned long wait_ui = now;

  if ((uint32_t) (now >= wait_ui)) {
    cli();
    wait_ui = now + 1000L;
    //serial::println::uint32((uint32_t) test_counter);
    //test_counter = 0;

    if (test_counter++ > 2) {
      test_counter = 0;
      DHT22::single::instance().m_needs_updating = true;
    }

    sei();
  }
}
