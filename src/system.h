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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/*
 * Timer1 heartbeat in milliseconds
 * Note: Changing the value here is not enough, timer1 must be updated also.
 */
#define HEARTBEAT 2


// Arduheater state machine bit map -------------------------------------------
// See system_t structure got more information
#define RUNNING               bit(0) // bitmask 00000001
#define ADC_READING_DONE      bit(1) // bitmask 00000010

#define NTC0_SENSOR_READY     bit(0) // bitmask 00000001
#define NTC1_SENSOR_READY     bit(1) // bitmask 00000010
#define NTC2_SENSOR_READY     bit(2) // bitmask 00000100
#define NTC3_SENSOR_READY     bit(3) // bitmask 00001000
#define AMBIENT_SENSOR_READY  bit(4) // bitmask 00010000

#define OUTPUT0_ENABLED       bit(0) // bitmask 00000001
#define OUTPUT1_ENABLED       bit(1) // bitmask 00000010
#define OUTPUT2_ENABLED       bit(2) // bitmask 00000100
#define OUTPUT3_ENABLED       bit(3) // bitmask 00001000


// Sensor bit map -------------------------------------------------------------
#define SENSOR_NEEDS_WARMUP   bit(0)  // bitmask 00000001
#define SENSOR_NEEDS_SLEEP    bit(1)  // bitmask 00000010
#define SENSOR_NEEDS_REFRESH  bit(2)  // bitmask 00000100

#endif
