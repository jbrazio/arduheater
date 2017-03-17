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

#include "common.h"

/*
 * Timer1 heartbeat in milliseconds
 * Note: Changing the value here is not enough, timer1 must be updated also.
 */
#define HEARTBEAT 50

namespace timer1 {
  /**
  * Enable the timer1 ISR in compare match mode with a 20Hz rate
  */
  inline void init() {
    DEBUGPRN(6, "timer1::init()");

    cli();
    TCCR1A = TCCR1B = TCNT1 = 0;
    OCR1A   = 0xC35;        // Compare match register
    TCCR1B |= bit(WGM12);   // CTC mode
    TCCR1B |= bit(CS12);    // 256 prescaler
    TIMSK1 |= bit(OCIE1A);  // enable timer compare interrupt
    sei();
  }
};

#endif
