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

#ifndef __STRINGS_H__
#define __STRINGS_H__

#include "arduheater.h"

// Special characters
const char string_colon             [] PROGMEM = { 0x3a, 0x00 };
const char string_comma             [] PROGMEM = { 0x2c, 0x00 };
const char string_eol               [] PROGMEM = { 0x0a, 0x00 };
const char string_minus             [] PROGMEM = { 0x2d, 0x00 };
const char string_percent           [] PROGMEM = { 0x25, 0x00 };
const char string_space             [] PROGMEM = { 0x20, 0x00 };
const char string_zero              [] PROGMEM = { 0x30, 0x00 };

// Serial strings
const char string_serial_start      [] PROGMEM = { "is starting" };
const char string_no_pci            [] PROGMEM = { "PIN is not interrupt-capable" };


// LCD strig
const char string_lcd_ambient       [] PROGMEM = { "Ambient" };
const char string_lcd_dew_point     [] PROGMEM = { "Dew point" };
const char string_lcd_humidity      [] PROGMEM = { "Humidity" };
const char string_lcd_not_apply     [] PROGMEM = { "N/A" };
const char string_lcd_unit_C        [] PROGMEM = { "C" };
const char string_lcd_please_wait   [] PROGMEM = { "Please wait.." };

#endif
