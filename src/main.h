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

#ifndef __ARDUHEATER_H__
#define __ARDUHEATER_H__

#include <Arduino.h>
#include <u8glib.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "assert.h"

#include "types.h"
#include "macros.h"
#include "version.h"
#include "strings.h"
#include "print.h"
#include "observer.h"
#include "subject.h"
#include "enum.h"
#include "struct.h"
#include "singleton.h"
#include "runtime.h"
#include "keypad.h"
#include "ui.h"
#include "pcint.h"
#include "timer1.h"

#endif