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

#ifndef __CONFIG_H__
#define __CONFIG_H__

// ----------------------------------------------------------------------------
// Activate debug -------------------------------------------------------------
// ----------------------------------------------------------------------------
#define DEBUG
#define SCOPE_DEBUG


// ----------------------------------------------------------------------------
// OUTPUT CHANNEL configuration -----------------------------------------------
// ----------------------------------------------------------------------------
// Pins  5 and  6: controlled by timer0
// Pins  9 and 10: controlled by timer1
// Pins 11 and  3: controlled by timer2

#define CHANNEL1_OUTPUT 11 // PWM
#define CHANNEL1_SENSOR 0  // Analog

#define CHANNEL2_OUTPUT 6  // PWM
#define CHANNEL2_SENSOR 1  // Analog

#define CHANNEL3_OUTPUT 5  // PWM
#define CHANNEL3_SENSOR 2  // Analog

#define CHANNEL4_OUTPUT 3  // PWM
#define CHANNEL4_SENSOR 3  // Analog


// ----------------------------------------------------------------------------
// AMBIENT SENSOR configuration -----------------------------------------------
// ----------------------------------------------------------------------------
#define AMBIENT_SENSOR_TYPE DHT22 // Select the ambient sensor type
#define AMBIENT_SENSOR_PIN      2 // Digital PIN where ther sensor is connected

#define AMBIENT_SENSOR_T_LPF_ALPHA  0.3F  // Select a value between 0 and 1 where
                                          // 0 disables the sensor's temperature low
                                          // pass filter.

#define AMBIENT_SENSOR_H_LPF_ALPHA  0.8F  // Select a value between 0 and 1 where
                                          // 0 disables the sensor's humidity low
                                          // pass filter.

#define USE_NOAA_DEW_FUNCTION             // NOAA dew func will give you precision at
                                          // the cost of speed.

// WARNING: If you need to change any of the following options it is suggested
//          to open a support ticket[1] first.
//
// [1]: https://github.com/jbrazio/arduheater/issues
#define AMBIENT_SENSOR_ERROR_THRESHOLD 10  // Tolerable number of consecutive errors


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

// You shouldn't need to change the following values, if for some reason you
// really need to please report back[1] to the project team.
//
// [1]: https://github.com/jbrazio/arduheater/issues
#define NTC_MIN_RAW_VALUE     51 // min value to be considered a valid reading ()
#define NTC_MAX_RAW_VALUE    950 // max value to be considered a valid reading (-23C)
#define NTC_ERR_RAW_THRESHOLD 20 // used for min/max edge calculation
#define NTC_MAX_ERRORS        10 // tolerable number of consecutive errors


// EXPERIMENTAL FEATURE
// Please report back[1] to the project team if you experience issues with the
// following directive enabled.
//
// [1]: https://github.com/jbrazio/arduheater/issues
#define ADC_PRESCALER16


// ----------------------------------------------------------------------------
// PID configuration ----------------------------------------------------------
// ----------------------------------------------------------------------------
// The following directives control the default PID values for each component
// of the algorithm, chansing them here requires you to reset to the EEPROM
// to defaults by using the serial command "XXXX".
#define DEFAULT_Kp  10.0F
#define DEFAULT_Ki  00.3F
#define DEFAULT_Kd  00.0F


// ----------------------------------------------------------------------------
// Configuration validation ---------------------------------------------------
// ----------------------------------------------------------------------------
#include "assert.h"

#endif
