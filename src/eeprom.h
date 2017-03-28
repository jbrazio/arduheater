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
 * 0x10 float     Heater #0 temperature offset
 * 0x15 uint8_t   Heater #0 PID min value
 * 0x16 uint8_t   Heater #0 PID max value
 * 0x1a float     Heater #0 PID Kp
 * 0x1e float     Heater #0 PID Ki
 * 0x22 float     Heater #0 PID Kd
 *
 * 0x26 uint8_t   Heater #1 autostart
 * 0x27 float     Heater #1 temperature offset
 * 0x2b uint8_t   Heater #1 PID min value
 * 0x2c uint8_t   Heater #1 PID max value
 * 0x2d float     Heater #1 PID Kp
 * 0x31 float     Heater #1 PID Ki
 * 0x35 float     Heater #1 PID Kd
 *
 * 0x39 uint8_t   Heater #2 autostart
 * 0x3a float     Heater #2 temperature offset
 * 0x3e uint8_t   Heater #2 PID min value
 * 0x3f uint8_t   Heater #2 PID max value
 * 0x40 float     Heater #2 PID Kp
 * 0x44 float     Heater #2 PID Ki
 * 0x48 float     Heater #2 PID Kd
 *
 * 0x4c uint8_t   Heater #3 autostart
 * 0x4d float     Heater #3 temperature offset
 * 0x51 uint8_t   Heater #3 PID min value
 * 0x52 uint8_t   Heater #3 PID max value
 * 0x53 float     Heater #3 PID Kp
 * 0x57 float     Heater #3 PID Ki
 * 0x5b float     Heater #3 PID Kd
 *
 * 0x5f uint8_t   End byte 0xff
 *
 */

#define EEPROM_VERSION  0x011
#define EEPROM_OFFSET   0x10

#define EEPROM_ADDR_HEADER  (EEPROM_OFFSET       + 0x00)
#define EEPROM_ADDR_VERSION (EEPROM_ADDR_HEADER  + sizeof(uint16_t))
#define EEPROM_ADDR_AMBIENT (EEPROM_ADDR_VERSION + sizeof(uint8_t))
#define EEPROM_ADDR_HEATER0 (EEPROM_ADDR_AMBIENT + sizeof(ambient_t))
#define EEPROM_ADDR_HEATER1 (EEPROM_ADDR_HEATER0 + sizeof(heater_t))
#define EEPROM_ADDR_HEATER2 (EEPROM_ADDR_HEATER1 + sizeof(heater_t))
#define EEPROM_ADDR_HEATER3 (EEPROM_ADDR_HEATER2 + sizeof(heater_t))
#define EEPROM_ADDR_END     (EEPROM_ADDR_HEATER3 + sizeof(heater_t))

namespace eeprom {
  void defaults();
  void load();
  void save();
  void sync();
};

#endif
