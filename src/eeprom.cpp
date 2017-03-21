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

#include "arduheater.h"

void eeprom::read(void* src, const uint16_t& dst, const size_t& n) {
  eeprom_read_block(src, (void*) dst, n);
}

void eeprom::write(const void* src, const uint16_t& dst, const size_t& n) {
  eeprom_update_block(src, (void*) dst, n);
}

void eeprom::save() {
  // magic header
  eeprom_write_word((uint16_t*) EEPROM_OFFSET + 0x00, 0x4148);

  // layout version
  eeprom_write_byte((uint8_t*) EEPROM_OFFSET + 0x02, EEPROM_VERSION);

  // ambient config
  eeprom_update_block(&amb.config, (void *)(EEPROM_OFFSET + 0x03), sizeof(ambient_t));

  // heaters config
  eeprom_update_block(&out[0].config, (void *)(EEPROM_OFFSET + 0x1f), sizeof(heater_t));
  eeprom_update_block(&out[1].config, (void *)(EEPROM_OFFSET + 0x2f), sizeof(heater_t));
  eeprom_update_block(&out[2].config, (void *)(EEPROM_OFFSET + 0x3f), sizeof(heater_t));
  eeprom_update_block(&out[3].config, (void *)(EEPROM_OFFSET + 0x4f), sizeof(heater_t));
}

void eeprom::load() {
  if (eeprom_read_word((uint16_t*) EEPROM_OFFSET) != 0x4148) {
    serial::println::PGM(PSTR("err: eeprom read error"));
    return;
  }

  if (eeprom_read_byte((uint8_t*) EEPROM_OFFSET + 0x02) != EEPROM_VERSION) {
    serial::println::PGM(PSTR("err: eeprom layout missmatch"));
    return;
  }
}
