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

#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "arduheater.h"

/**
 * ADDR SIZE      DESCRIPTION
 * ---- --------- -------------------------------------------------------------
 * 0x00 uint16_t  Magic header aka "AH"
 * 0x02 uint8_t   EEMPROM layout version
 *
 * 0x03 float     Ambient temperature offset
 * 0x07 float     Ambient relative humidity offset
 * 0x0b float     Ambient dew offset
 *
 * 0x0f uint8_t   Heater #0 autostart
 * 0x10 uint8_t   Heater #0 temperature offset
 * 0x11 uint8_t   Heater #0 PID min value
 * 0x12 uint8_t   Heater #0 PID max value
 * 0x13 float     Heater #0 PID Kp
 * 0x17 float     Heater #0 PID Ki
 * 0x1b float     Heater #0 PID Kd
 *
 * 0x1f uint8_t   Heater #1 autostart
 * 0x20 uint8_t   Heater #1 temperature offset
 * 0x21 uint8_t   Heater #1 PID min value
 * 0x22 uint8_t   Heater #1 PID max value
 * 0x23 float     Heater #1 PID Kp
 * 0x27 float     Heater #1 PID Ki
 * 0x2b float     Heater #1 PID Kd
 *
 * 0x2f uint8_t   Heater #2 autostart
 * 0x30 uint8_t   Heater #2 temperature offset
 * 0x31 uint8_t   Heater #2 PID min value
 * 0x32 uint8_t   Heater #2 PID max value
 * 0x33 float     Heater #2 PID Kp
 * 0x37 float     Heater #2 PID Ki
 * 0x3b float     Heater #2 PID Kd
 *
 * 0x3f uint8_t   Heater #3 autostart
 * 0x40 uint8_t   Heater #3 temperature offset
 * 0x41 uint8_t   Heater #3 PID min value
 * 0x42 uint8_t   Heater #3 PID max value
 * 0x43 float     Heater #3 PID Kp
 * 0x47 float     Heater #3 PID Ki
 * 0x4b float     Heater #3 PID Kd
 *
 */

#define EEPROM_VERSION  1
#define EEPROM_OFFSET   10

namespace eeprom {
  void read(void* src, const uint16_t& dst, const size_t& n);
  void write(const void* src, const uint16_t& dst, const size_t& n);
  void save();
  void load();
};

#endif
