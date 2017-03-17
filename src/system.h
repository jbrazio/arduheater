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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "arduheater.h"

/*
 * Timer1 heartbeat in milliseconds
 * Note: Changing the value here is not enough, timer1 must be updated also.
 */
#define HEARTBEAT 50

// Arduheater state machine bit map -------------------------------------------
#define RUNNING             bit(0)  // bitmask 00000001
#define ADC_READING_DONE    bit(1)  // bitmask 00000010

// Internal module status bit map ---------------------------------------------
// Each bit represents a boolean status for the module readiness
#define STATUS_SERIAL_READY   bit(0)  // bitmask 00000001
#define STATUS_ISR_READY      bit(1)  // bitmask 00000010
#define STATUS_KEYPAD_READY   bit(2)  // bitmask 00000100
#define STATUS_LCD_READY      bit(3)  // bitmask 00001000
#define STATUS_AMBIENT_READY  bit(4)  // bitmask 00010000
#define STATUS_HEATER_READY   bit(5)  // bitmask 00100000
#define STATUS_THERMAL_READY  bit(6)  // bitmask 01000000

// Sensor bit map -------------------------------------------------------------
#define SENSOR_NEEDS_WARMUP   bit(0)  // bitmask 00000001
#define SENSOR_NEEDS_SLEEP    bit(1)  // bitmask 00000010
#define SENSOR_NEEDS_REFRESH  bit(2)  // bitmask 00000100

#endif
