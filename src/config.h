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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "arduheater.h"

// ----------------------------------------------------------------------------
// Activate debug -------------------------------------------------------------
// ----------------------------------------------------------------------------
//#define DEBUG

// ----------------------------------------------------------------------------
// SERIAL PORT configuration --------------------------------------------------
// ----------------------------------------------------------------------------

// Defines the max number of received chars over the serial line that can be
// waiting to be processed, if the buffer gets full incoming data will be lost.
#define SERIAL_RX_BUFFER_SIZE 16

// Defines the max number of chars waiting to be sent over the serial line.
#define SERIAL_TX_BUFFER_SIZE 64

// Defines the speed at which the serial line will operate.
// The default settings are: 8-bit, no parity, 1 stop bit.
#define SERIAL_BAUDRATE       57600

// Defines the char which will represent a null buffer value, do not change this.
#define SERIAL_NO_DATA        0xff

// Defines the max length of a received command string.
#define COMMAND_BUFFER_SIZE   16


// ----------------------------------------------------------------------------
// OUTPUT CHANNEL configuration -----------------------------------------------
// ----------------------------------------------------------------------------

// Defines the number of heater outputs supported by this configuration, it
// should be defined as a integer value between 1 and 4.
// (PWM: 3, 5, 6, 9, 10, and 11)
// Pins 5 and 6: controlled by timer0
// Pins 9 and 10: controlled by timer1
// Pins 11 and 3: controlled by timer2
#define NUM_OUTPUTS 4

// The list of PIN to be used as heater outputs
// Channel:           #0  #1  #2  #3
#define HEATER_PINS { 11,  6,  5,  3 }

// ----------------------------------------------------------------------------
// AMBIENT SENSOR configuration -----------------------------------------------
// ----------------------------------------------------------------------------
#define AMBIENT_PIN 2


// ----------------------------------------------------------------------------
// THERMISTOR configuration ---------------------------------------------------
// ----------------------------------------------------------------------------
#define THERMISTOR_NOMINAL_TEMP      25.0
#define THERMISTOR_BCOEFFICIENT    3950.0
#define THERMISTOR_NOMINAL_VAL    10000.0
#define THERMISTOR_SERIESRESISTOR 10000.0

#define THERMISTOR_MAX_VAL   270  // ~50C
#define THERMISTOR_MIN_VAL   935  // ~(-20C)
#define THERMISTOR_ERR_TEMP 1024

// Defines how many invalid readings we can receive from a thermistor
// before disabling the associated output channel.
#define THERMISTOR_ERR_THRESHOLD 16

#endif
