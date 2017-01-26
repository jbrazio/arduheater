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

enum message_type_t {
  MESSAGE_TYPE_UNKNOWN,
  MESSAGE_TYPE_BUTTON
};

enum key_type_t {
  KEY_TYPE_UNKNOWN,
  KEY_TYPE_LEFT,
  KEY_TYPE_RIGHT,
  KEY_TYPE_OK
};

enum keypress_t {
  KEYPRESS_UNKNOWN,
  KEYPRESS_SHORT,
  KEYPRESS_LONG,
};

enum menu_page_t {
  PAGE_UNKNOWN,
  PAGE_BLANK,
  PAGE_BOOTSCREEN,
  PAGE_HOME,
  PAGE_TEST_A,
  PAGE_TEST_B
};

enum menu_font_t {
  FONT_SMALL,
  FONT_BIG
};

#endif
