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

#include "eeprom.h"

/**
 * @brief Preinstantiate objects
 */
eeprom_map_t eemap;

bool eeprom_init(eeprom_map_t * ptr)
{
  eeprom_busy_wait();
  eeprom_load(ptr);

  if ((*ptr).header != EEPROM_MAGIC_HEADER)
  {
    memset(ptr, 0, sizeof(eeprom_map_t));
    (*ptr).header = EEPROM_MAGIC_HEADER;
    eeprom_save(ptr);
    return false;
  }

  return true;
}

void eeprom_load(eeprom_map_t * ptr)
{
  eeprom_read_block(ptr, EEPROM_START_ADDRESS, sizeof(eeprom_map_t));
}

void eeprom_save(eeprom_map_t * ptr)
{
  eeprom_update_block(ptr, EEPROM_START_ADDRESS, sizeof(eeprom_map_t));
}

void load_config()
{
  bool blank = eeprom_init(&eemap);

  if(blank) {
    eemap.output[3] = eemap.output[2] = eemap.output[1] = eemap.output[0]
      = { 0, 255, false, 0.0F, 0.0F, DEFAULT_Kp, DEFAULT_Ki, DEFAULT_Kd };

    eemap.ntc[0] = { CHANNEL1_NTC_NT, CHANNEL1_NTC_SR, CHANNEL1_NTC_BC, CHANNEL1_NTC_NV };
    eemap.ntc[1] = { CHANNEL2_NTC_NT, CHANNEL2_NTC_SR, CHANNEL2_NTC_BC, CHANNEL2_NTC_NV };
    eemap.ntc[2] = { CHANNEL3_NTC_NT, CHANNEL3_NTC_SR, CHANNEL3_NTC_BC, CHANNEL3_NTC_NV };
    eemap.ntc[3] = { CHANNEL4_NTC_NT, CHANNEL4_NTC_SR, CHANNEL4_NTC_BC, CHANNEL4_NTC_NV };

    eemap.ambient = { 0.0F, 0.0F };
  }

  ambient.import_config(eemap.ambient);

  for (size_t i = 0; i < 4; i++ ) {
    output[i].import_config(eemap.output[i]);
    output[i].import_config_ntc(eemap.ntc[i]);
  }
}

void save_config()
{
  eeprom_save(&eemap);
}
