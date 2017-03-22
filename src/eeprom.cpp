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

void eeprom::save() {
  // magic header
  eeprom_write_word((uint16_t*) EEPROM_ADDR_HEADER, 0x4148);

  // layout version
  eeprom_write_byte((uint8_t*) EEPROM_ADDR_VERSION, EEPROM_VERSION);

  // ambient config
  eeprom_update_block(&amb.config, (void *) EEPROM_ADDR_AMBIENT, sizeof(ambient_t));

  // heaters config
  eeprom_update_block(&out[0].config, (void *) EEPROM_ADDR_HEATER0, sizeof(heater_t));
  eeprom_update_block(&out[1].config, (void *) EEPROM_ADDR_HEATER1, sizeof(heater_t));
  eeprom_update_block(&out[2].config, (void *) EEPROM_ADDR_HEATER2, sizeof(heater_t));
  eeprom_update_block(&out[3].config, (void *) EEPROM_ADDR_HEATER3, sizeof(heater_t));

  // end
  eeprom_write_byte((uint8_t*) EEPROM_ADDR_END, 0xff);
}

void eeprom::load() {
  if (eeprom_read_word((uint16_t*) EEPROM_ADDR_HEADER) != 0x4148) {
    defaults();
    save();
  }
  else if (eeprom_read_byte((uint8_t*) EEPROM_ADDR_VERSION) != EEPROM_VERSION) {
    defaults();
    save();
  }
  else {
    // ambient config
    eeprom_read_block(&amb.config, (void *) EEPROM_ADDR_AMBIENT, sizeof(ambient_t));

    // heaters config
    eeprom_read_block(&out[0].config, (void *) EEPROM_ADDR_HEATER0, sizeof(heater_t));
    eeprom_read_block(&out[1].config, (void *) EEPROM_ADDR_HEATER1, sizeof(heater_t));
    eeprom_read_block(&out[2].config, (void *) EEPROM_ADDR_HEATER2, sizeof(heater_t));
    eeprom_read_block(&out[3].config, (void *) EEPROM_ADDR_HEATER3, sizeof(heater_t));
  }

  // TODO: currently we do nothing with the extra ambient sensor config
  for (size_t i = 0; i < NUM_OUTPUTS; i++ ) {
    out[i].alg.limit(out[i].config.min, out[i].config.max);
    out[i].alg.tune(out[i].config.Kp, out[i].config.Ki, out[i].config.Kd);
  }
}

void eeprom::defaults() {
  //serial::println::PGM(PSTR("warn: default settings loaded"));
  amb.config = { AMBIENT_T_OFFSET, AMBIENT_RH_OFFSET, AMBIENT_DEW_OFFSET };
  for (size_t i = 0; i < NUM_OUTPUTS; i++ ) { out[i].config = { false, 0, 0, 255, 10, 0, 10 }; }
}
