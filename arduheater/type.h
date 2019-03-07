/**
 * Arduheater - Heat controller for astronomy usage
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

#ifndef __TYPE_H__
#define __TYPE_H__

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t  pin_t;
typedef uint8_t  flag_t;
typedef uint32_t micros_t;

typedef struct {
  uint8_t min, max;
  bool    autostart;
  float   temp_offset, setpoint_offset, Kp, Ki, Kd;
} output_config_t;

typedef struct {
  float    nominaltemp;
  uint16_t resistor, bcoefficient, nominalval;
} output_config_ntc_t;

typedef struct {
  float temp_offset, rh_offset;
} environment_config_t;

typedef struct {
  uint16_t                header;
  output_config_t      output[4];
  output_config_ntc_t     ntc[4];
  environment_config_t   ambient;
} eeprom_map_t;

#endif
