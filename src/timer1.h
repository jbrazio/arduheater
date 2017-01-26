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

#ifndef __TIMER1_H__
#define __TIMER1_H__

namespace timer1 {
  /**
  * Enable the timer1 ISR in compare match mode with a 20Hz rate
  */
  void init() {
    cli();
    TCCR1A = TCCR1B = TCNT1 = 0;

    OCR1A   = 0xC35;          // Compare match register
    TCCR1B |= (1 << WGM12);   // CTC mode
    TCCR1B |= (1 << CS12);    // 256 prescaler
    TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
    sei();
  }

  /**
   * Timer1 interrupt handler
   */
  ISR(TIMER1_COMPA_vect) {
    static volatile uint8_t busy = false;  // Binary semaphore
    if (busy) { return; }                  // Prevent ISR (re)triggering
    busy = true;                           // Acquire the lock

    keypad::single::instance().worker();
    ui::single::instance().worker();

    busy = false;                   // Release the lock
    //PORTB ^= 0x20;                // Set D13 low
  }
};

#endif
