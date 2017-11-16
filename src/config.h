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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <avr/pgmspace.h>
#include "type.h"

// ----------------------------------------------------------------------------
// Activate debug -------------------------------------------------------------
// ----------------------------------------------------------------------------
//#define DEBUG


#define ALLOW_HUMAN_ON_SERIAL


// ----------------------------------------------------------------------------
// OUTPUT CHANNEL configuration -----------------------------------------------
// ----------------------------------------------------------------------------
// Pins  5 and  6: controlled by timer0
// Pins  9 and 10: controlled by timer1
// Pins 11 and  3: controlled by timer2

// The list of PIN to be used as heater outputs
// Channel:                           #1  #2  #3  #4
const pin_t heater_pins[] PROGMEM = { 11,  6,  5,  3 }; // Digital
const pin_t sensor_pins[] PROGMEM = {  0,  1,  2,  3 }; // Analog

// ----------------------------------------------------------------------------
// AMBIENT SENSOR configuration -----------------------------------------------
// ----------------------------------------------------------------------------
#define AMBIENT_PIN 2


// ----------------------------------------------------------------------------
// THERMISTOR configuration ---------------------------------------------------
// ----------------------------------------------------------------------------
// The directives bellow have standard values for the most used NTC thermistor
// type, if you need to tweak them bare in mind that all the output channels
// will be impacted.
#define DEFAULT_NTC_NT  25.0F // Nominal temperature
#define DEFAULT_NTC_BC  3950L // B coeficient
#define DEFAULT_NTC_NV 10000L // Nominal value
#define DEFAULT_NTC_SR 10000L // Series resistor value


#define THERMISTOR_MAX_VAL   270  // ~50C
#define THERMISTOR_MIN_VAL   935  // ~(-20C)
#define THERMISTOR_ERR_TEMP 1023

// EXPERIMENTAL FEATURE
// Please report back[1] to the project team if you experience issues with the
// following directive enabled.
//
// [1]: https://github.com/jbrazio/arduheater/issues
#define ADC_PRESCALER16

// Defines how many invalid readings we can receive from a thermistor
// before disabling the associated output channel.
#define THERMISTOR_ERR_THRESHOLD 16


#define DEFAULT_Kp 1.0F
#define DEFAULT_Ki 0.03F
#define DEFAULT_Kd 5.00F

#endif
