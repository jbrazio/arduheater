/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2018 João Brázio [joao@brazio.org]
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

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include <avr/interrupt.h>

#include "macro.h"

extern volatile uint32_t timer0_overflow_count;

/**
 * @brief Number of elapsed microseconds since boot
 * @details
 *
 */
inline uint32_t micros() {
  CRITICAL_SECTION_START
  uint8_t  a = TCNT0;
  uint32_t b = timer0_overflow_count;
  if ((TIFR0 & bit(TOV0)) && (a < 255)) ++b;
  CRITICAL_SECTION_END

  return ((b << 8) + a) << 2;
}

float calculate_dew(const float&, const float&);

#endif
