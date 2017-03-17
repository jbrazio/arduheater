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

#ifndef __ARDUHEATER_H__
#define __ARDUHEATER_H__

 // Program version and release
#define ARDUHEATER_VERSION "0.1a"
#define ARDUHEATER_VERSION_BUILD "20170316"

// Define standard libraries used by Arduheater
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
//#include <avr/wdt.h>
//#include <util/delay.h>
#include <math.h>
//#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
//#include <stdint.h>
//#include <stdbool.h>

#include <Arduino.h>

// Cleanup some Arduino stuff
#undef SERIAL_RX_BUFFER_SIZE
#undef SERIAL_TX_BUFFER_SIZE

// Define the Arduheater system include files.
// NOTE: Do not alter organization.
#include "config.h"
#include "macros.h"
#include "ringbuf.h"
#include "types.h"
#include "enum.h"
#include "struct.h"
#include "utils.h"
#include "system.h"
#include "serial.h"
#include "strings.h"
#include "print.h"
#include "movingmean.h"
#include "adc.h"
#include "sensor.h"
#include "thermistor.h"
#include "dht22.h"


extern system_t     sys;
extern thermistor   ntc;
extern dht22        amb;

#endif
