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

#ifndef __COMMON_H__
#define __COMMON_H__

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <U8glib.h>

#include "config.h"
#include "version.h"
#include "strings.h"

#include "assert.h"

#include "types.h"
#include "enum.h"
#include "struct.h"
#include "circularqueue.h"
#include "smooth_mm.h"

#include "serial.h"
#include "print.h"
#include "macros.h"
#include "timer1.h"
#include "utils.h"

#include "observer.h"
#include "subject.h"
#include "singleton.h"

#include "card.h"
#include "sensor.h"
#include "dht22.h"
#include "keypad.h"
#include "painter.h"
#include "pid.h"
#include "runtime.h"
#include "thermistor.h"
#include "ui.h"

#include "bitmaps/bootlogo.h"

#include "widgets/widgets.h"

#include "cards/blank.h"
#include "cards/home.h"
#include "cards/splash.h"

#endif
