/**
 * Arduheater - Telescope heat controller
 * Copyright (C) 2016 João Brázio [joao@brazio.org]
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

#ifndef __ENUM_H__
#define __ENUM_H__

enum output_type_t {
  OUTPUT_UNKNOWN,
  OUTPUT_WITH_SENSOR,
  OUTPUT_WITHOUT_SENSOR
};

enum sensor_type_t {
  SENSOR_UNKNOWN,
  SENSOR_NONE,
  SENSOR_NTC_100K_1_3950,
  SENSOR_NTC_010K_5_3950,
  SENSOR_DS18B20,
  SENSOR_DHT22
};

enum menu_page_t {
  PAGE_UNKNOWN,
  PAGE_BLANK,
  PAGE_BOOTSCREEN,
  PAGE_HOME,
  COUNT_menu_page_t
};

enum key_state_t {
  KEY_UNKNOWN,
  KEY_PRESSED,
  KEY_RELEASED
};

enum key_type_t {
  KEY_NONE,
  KEY_LEFT,
  KEY_RIGHT
};

#endif
