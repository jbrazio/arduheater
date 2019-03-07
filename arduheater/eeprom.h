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

#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>

#include "version.h"
#include "config.h"

#include "type.h"
extern eeprom_map_t eemap;

#include "output.h"
#include "environment.h"

#define EEPROM_MAGIC_HEADER  0xab56
#define EEPROM_START_ADDRESS 0x0000

bool eeprom_init(eeprom_map_t *);
void eeprom_load(eeprom_map_t *);
void eeprom_save(eeprom_map_t *);

void load_config();
void save_config();

#endif
