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

#ifndef __DEBUG_H__
#define __DEBUG_H__
#ifdef DEBUG___

  #include <avr/pgmspace.h>

  static void debugP(const char *str) {
    char c;

    while ((c = pgm_read_byte(str))) {
      Serial.write(c);
      str++;
    }
  }

  static void debugLineP(const char *str) {
    debugP(str);
    Serial.write("\n");
  }

#endif
#endif
