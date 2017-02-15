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

#ifndef __TIMER1_H__
#define __TIMER1_H__

#include <Arduino.h>
#include "dht22.h"
#include "keypad.h"
#include "timer1.h"
#include "ui.h"

/**
 * Timer1 interrupt handler
 */
ISR(TIMER1_COMPA_vect) {
  static volatile uint8_t busy = false; // Binary semaphore
  if (busy) { return; }                 // Prevent ISR (re)triggering
  PORTB |= 0x20;                        // Set D13 high
  busy = true;                          // Acquire the lock

  keypad::single::instance().irq();
  DHT22::single::instance().irq();
  ui::single::instance().irq();

  busy = false;   // Release the lock
  PORTB &= ~0x20; // Set D13 low
}

#endif
