/**
 * Arduheater - Telescope heat controller
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

#ifndef __ENUM_H__
#define __ENUM_H__

enum msg_category_t {
  MSG_CAT_UNKNOWN,
  MSG_CAT_KEYPAD,
  MSG_UPDATE_LCD,
  MSG_CAT_WEATHER,
  MSG_CAT_NTC
};

enum keycode_t {
  KEYCODE_UNKNOWN,
  KEYCODE_LEFT,
  KEYCODE_RIGHT,
  KEYCODE_OK,
};

enum keypress_t {
  KEYPRESS_UNKNOWN,
  KEYPRESS_SHORT,
  KEYPRESS_LONG,
};

enum card_index_t {
  CARD_BLANK,
  CARD_SPLASH,
  CARD_HOME,
};

enum sensor_state_t {
  SENSOR_UNKNOWN,
  SENSOR_WARMUP,
  SENSOR_READY,
  SENSOR_BUSY,
  SENSOR_SLEEP,
  SENSOR_TIMEOUT,
  SENSOR_ERROR,
};

#endif
