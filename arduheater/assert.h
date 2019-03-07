/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2019 João Brázio [joao@brazio.org]
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

#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "version.h"
#include "config.h"

#include "type.h"

#if ! defined (__AVR_ATmega328P__) && ! defined (__AVR_ATmega328PB__) && ! defined (__AVR_ATmega168__) && ! defined (__AVR_ATmega168P__)
  #error Supported platforms: Arduino UNO, NANO, PRO (AVR ATmega 328P)
#endif

#ifndef AMBIENT_SENSOR_TYPE
  #error Please select the type of ambient sensor used
#else
  #if AMBIENT_SENSOR_TYPE == DHT22
    #if AMBIENT_SENSOR_PIN != 2 && AMBIENT_SENSOR_PIN != 3
      #error The ambient sensor DHT22 must be connected to an interupt pin (D2 or D3)
    #endif
  #endif
#endif

#if !defined(CHANNEL1_OUTPUT) || !defined(CHANNEL2_OUTPUT) || !defined(CHANNEL3_OUTPUT) || !defined(CHANNEL4_OUTPUT)
  #error Missing the output pin configuration for one of the channels
#else
  #if CHANNEL1_OUTPUT != 5 && CHANNEL1_OUTPUT != 6 && CHANNEL1_OUTPUT != 11 && CHANNEL1_OUTPUT != 3
    #error The output pin for channel one must be PWM capable
  #endif

  #if CHANNEL2_OUTPUT != 5 && CHANNEL2_OUTPUT != 6 && CHANNEL2_OUTPUT != 11 && CHANNEL2_OUTPUT != 3
    #error The output pin for channel two must be PWM capable
  #endif

  #if CHANNEL3_OUTPUT != 5 && CHANNEL3_OUTPUT != 6 && CHANNEL3_OUTPUT != 11 && CHANNEL3_OUTPUT != 3
    #error The output pin for channel three must be PWM capable
  #endif

  #if CHANNEL4_OUTPUT != 5 && CHANNEL4_OUTPUT != 6 && CHANNEL4_OUTPUT != 11 && CHANNEL4_OUTPUT != 3
    #error The output pin for channel four must be PWM capable
  #endif

  const pin_t heater_pins[] PROGMEM = {
    CHANNEL1_OUTPUT,
    CHANNEL2_OUTPUT,
    CHANNEL3_OUTPUT,
    CHANNEL4_OUTPUT
  };
#endif

#if !defined(CHANNEL1_SENSOR) || !defined(CHANNEL2_SENSOR) || !defined(CHANNEL3_SENSOR) || !defined(CHANNEL4_SENSOR)
  #error Missing the output pin configuration for one of the channels
#else
  const pin_t sensor_pins[] PROGMEM = {
    CHANNEL1_SENSOR,
    CHANNEL2_SENSOR,
    CHANNEL3_SENSOR,
    CHANNEL4_SENSOR
  };
#endif

// Channel 1 NTC settings
#ifndef CHANNEL1_NTC_NT
  #define CHANNEL1_NTC_NT DEFAULT_NTC_NT
#endif

#ifndef CHANNEL1_NTC_BC
  #define CHANNEL1_NTC_BC DEFAULT_NTC_BC
#endif

#ifndef CHANNEL1_NTC_NV
  #define CHANNEL1_NTC_NV DEFAULT_NTC_NV
#endif

#ifndef CHANNEL1_NTC_SR
  #define CHANNEL1_NTC_SR DEFAULT_NTC_SR
#endif

// Channel 2 NTC settings
#ifndef CHANNEL2_NTC_NT
  #define CHANNEL2_NTC_NT DEFAULT_NTC_NT
#endif

#ifndef CHANNEL2_NTC_BC
  #define CHANNEL2_NTC_BC DEFAULT_NTC_BC
#endif

#ifndef CHANNEL2_NTC_NV
  #define CHANNEL2_NTC_NV DEFAULT_NTC_NV
#endif

#ifndef CHANNEL2_NTC_SR
  #define CHANNEL2_NTC_SR DEFAULT_NTC_SR
#endif

// Channel 3 NTC settings
#ifndef CHANNEL3_NTC_NT
  #define CHANNEL3_NTC_NT DEFAULT_NTC_NT
#endif

#ifndef CHANNEL3_NTC_BC
  #define CHANNEL3_NTC_BC DEFAULT_NTC_BC
#endif

#ifndef CHANNEL3_NTC_NV
  #define CHANNEL3_NTC_NV DEFAULT_NTC_NV
#endif

#ifndef CHANNEL3_NTC_SR
  #define CHANNEL3_NTC_SR DEFAULT_NTC_SR
#endif

// Channel 4 NTC settings
#ifndef CHANNEL4_NTC_NT
  #define CHANNEL4_NTC_NT DEFAULT_NTC_NT
#endif

#ifndef CHANNEL4_NTC_BC
  #define CHANNEL4_NTC_BC DEFAULT_NTC_BC
#endif

#ifndef CHANNEL4_NTC_NV
  #define CHANNEL4_NTC_NV DEFAULT_NTC_NV
#endif

#ifndef CHANNEL4_NTC_SR
  #define CHANNEL4_NTC_SR DEFAULT_NTC_SR
#endif

#endif
