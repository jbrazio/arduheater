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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

//#include <math.h>

#include "heater.h"
#include "thermistor.h"

#define EEPROM_VERSION  0x01
#define EEPROM_OFFSET   0x10

// Memory map
const uint16_t eeprom_map[] PROGMEM =
{
  sizeof(uint16_t),
  sizeof(Thermistor::config_t),
  //sizeof(Heater::config_t),
  sizeof(uint32_t)
};

#endif
