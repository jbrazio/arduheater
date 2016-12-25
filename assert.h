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

#ifndef __ASSERT_H__
#define __ASSERT_H__

#include "main.h"

/**
 * Validate the micro processor
 */
#if !defined(__AVR_ATmega328__) && !defined(__AVR_ATmega328P__)
  #error "Only ATmega328 boards are currently supported"
#endif

/**
 * Assert that the user selected keys are on valid PCI pins
 */
static_assert((digitalPinToPCICR(UI_KEYPAD_A_PIN) != NULL), "UI_KEYPAD_A_PIN is not interrupt-capable");
static_assert((digitalPinToPCICR(UI_KEYPAD_B_PIN) != NULL), "UI_KEYPAD_B_PIN is not interrupt-capable");

#endif
