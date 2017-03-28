/**
 * Arduheater - Heat controller for astronomy usage
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

#ifndef __MACROS_H__
#define __MACROS_H__

#include "arduheater.h"

#ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#ifndef bit
  #define bit(n) (1 << n)
#endif

#ifndef array_size
  #define array_size(a) (sizeof(a) / sizeof(*a))
#endif

#ifndef no_more
  #define no_more(var, high) do { if (var > high) var = high; } while(0)
#endif

#ifndef no_less
  #define no_less(var, low) do { if (var < low) var = low; } while(0)
#endif

#ifndef output_pin
  #define output_pin(N) (pgm_read_byte(heater_ouput_pins + (N)))
#endif

#ifndef ntc_ready
  #define ntc_ready(N) (sys.status & STATUS_NTC0_READY << N)
#endif

#ifndef any_ntc_ready
  #if (NUM_OUTPUTS > 3)
    #define any_ntc_ready() (ntc_ready(0) || ntc_ready(1) || ntc_ready(2) || ntc_ready(3))
  #elif (NUM_OUTPUTS > 2)
    #define any_ntc_ready() (ntc_ready(0) || ntc_ready(1) || ntc_ready(2))
  #elif (NUM_OUTPUTS > 1)
    #define any_ntc_ready() (ntc_ready(0) || ntc_ready(1))
  #elif (NUM_OUTPUTS > 0)
    #define any_ntc_ready() (ntc_ready(0))
  #endif
#endif

#ifndef disable_all_outputs
  #define disable_all_outputs() do {  \
    digitalWrite(output_pin(0), LOW); \
    digitalWrite(output_pin(1), LOW); \
    digitalWrite(output_pin(2), LOW); \
    digitalWrite(output_pin(3), LOW); } while(0)
#endif

#ifndef halt
  #define halt() do {                                 \
    cli();                                            \
    disable_all_outputs();                            \
    serial::println::PGM(PSTR("err: system halted")); \
    serial::flush();                                  \
    while(1) { utils::delay(1); } } while(0)
#endif

#ifndef ARDUINO
  // the prescaler is set so that timer0 ticks every 64 clock cycles, and the
  // the overflow handler is called every 256 ticks.
  #define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

  // the whole number of milliseconds per timer0 overflow
  #define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

  // the fractional number of milliseconds per timer0 overflow. we shift right
  // by three to fit these numbers into a byte. (for the clock speeds we care
  // about - 8 and 16 MHz - this doesn't lose precision.)
  #define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
  #define FRACT_MAX (1000 >> 3)
#endif

#endif
