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

  pinMode(HEATER_0_PIN, OUTPUT);
  pinMode(HEATER_1_PIN, OUTPUT);
  pinMode(HEATER_2_PIN, OUTPUT);
  pinMode(HEATER_3_PIN, OUTPUT);

  ui::single::instance().show(CARD_SPLASH);

  serial::init();
  timer1::init();

  serial::print::banner();

  keypad::single::instance().attach(&runtime::single::instance());
  DHT22::single::instance().attach(&runtime::single::instance());
  thermistor::single::instance().attach(&runtime::single::instance());

  for (size_t i = 0; i < 1; i++) {
    runtime::single::instance().heater[i].pid.setpoint(50);
    runtime::single::instance().heater[i].pid.tune(25.00, 00.20, 15.00);
    //p->pid.mode(PID::AUTOMATIC);
  }

  /*while(runtime::single::instance().ambient.t() == 0
    || runtime::single::instance().ambient.rh() == 0
    || ! runtime::single::instance().heater[3].t.full()) {;}*/

  ui::single::instance().show(CARD_HOME);
}

void loop() {
  uint32_t now = millis();
  static unsigned long wait_ui = now;

  if ((uint32_t) (now >= wait_ui)) {
    wait_ui = now + 1000L;
  }

  ui::single::instance().draw();
}
