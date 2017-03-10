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

#include "common.h"

void setup() {
  DDRB |= 0x20; // Enable D13 as output

  serial::init();
  serial::print::banner();

  ui::single::instance().show(CARD_SPLASH, 250);

  keypad::single::instance().init(UI_KEYPAD_A_PIN, UI_KEYPAD_B_PIN);
  keypad::single::instance().attach(&runtime::single::instance());

  DHT22::single::instance().init(AMBIENT_PIN);
  DHT22::single::instance().attach(&runtime::single::instance());

  thermistor::single::instance().init();
  thermistor::single::instance().attach(&runtime::single::instance());

  timer1::init();


  pinMode(HEATER_A_PIN, OUTPUT);
  //TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);
  //TCCR0B = _BV(CS02) | _BV(CS00);
  //TCCR0B = _BV(CS02);
  //analogWrite(HEATER_A_PIN, 100);

  for (size_t i = 0; i < 1; i++) {
    runtime::output_t* p = &runtime::single::instance().m_output[i];
    p->pid.setpoint(50);
    p->pid.tune(25.00, 00.20, 15.00);
    //p->pid.mode(PID::AUTOMATIC);
  }
}

void loop() {
  uint32_t now = millis();
  static unsigned long wait_ui = now;

  if ((uint32_t) (now >= wait_ui)) {
    wait_ui = now + 1000L;
  }

  ui::single::instance().draw();
}
