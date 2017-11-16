/**
 * Arduheater - An intelligent dew buster for astronomy
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

#ifndef __ARDUHEATER_H__
#define __ARDUHEATER_H__

#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "version.h"
#include "console.h"
#include "analog.h"
#include "protocol.h"

#include "output.h"

/*
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )

#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )

inline void pinMode(uint8_t pin, uint8_t mode)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *reg, *out;

  if (port == 0) return;

  // JWS: can I let the optimizer do this?
  reg = portModeRegister(port);
  out = portOutputRegister(port);

  if (mode == 0x0) {
    uint8_t oldSREG = SREG;
                cli();
    *reg &= ~bit;
    *out &= ~bit;
    SREG = oldSREG;
  } else if (mode == 0x2) {
    uint8_t oldSREG = SREG;
                cli();
    *reg &= ~bit;
    *out |= bit;
    SREG = oldSREG;
  } else {
    uint8_t oldSREG = SREG;
                cli();
    *reg |= bit;
    SREG = oldSREG;
  }
}
*/
#endif
