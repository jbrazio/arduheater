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

#ifndef __PCINT_H__
#define __PCINT_H__

#include <Arduino.h>
#include "keypad.h"

ISR(PCINT0_vect)
{
  // #define PB 2
  keypad::single::instance().isr(2);
}

ISR(PCINT1_vect)
{
  // #define PC 3
  keypad::single::instance().isr(3);
}

ISR(PCINT2_vect)
{
  // #define PD 4
  keypad::single::instance().isr(4);
}

#endif
