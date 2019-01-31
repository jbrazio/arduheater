/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2019 João Brázio [joao@brazio.org]
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

#ifndef __MACRO_H__
#define __MACRO_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sfr_defs.h>

#undef bit
#define bit(b) (1UL << (b))

#undef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

#undef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#undef min
#define min(a,b) ((a)<(b)?(a):(b))

#undef max
#define max(a,b) ((a)>(b)?(a):(b))

#undef abs
#define abs(x) ((x)>0?(x):-(x))

#undef constrain
#define constrain(n, low, high) ((n)<(low)?(low):((n)>(high)?(high):(n)))

#undef map
#define map(x, in_min, in_max, out_min, out_max) ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

#undef asizeof
#define asizeof(a) (sizeof(a) / sizeof(*a))

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  const uint8_t __SREG___ = SREG; cli();
  #define CRITICAL_SECTION_END    SREG = __SREG___;
#endif

#define SCOPE_DEBUG_OUTPUT(a)
#define ENABLE_SCOPE_DEBUG(a)

#ifdef SCOPE_DEBUG
  #undef SCOPE_DEBUG_OUTPUT
  #undef ENABLE_SCOPE_DEBUG
  #define SCOPE_DEBUG_OUTPUT(a) PORTB ^= bit(a)
  #define ENABLE_SCOPE_DEBUG(a) IO::set_as_output(a); IO::write(a, LOW);
#endif

#define get_heater_pin(a) pgm_read_byte(&(heater_pins[a]))
#define get_sensor_pin(a) pgm_read_byte(&(sensor_pins[a]))

#endif
