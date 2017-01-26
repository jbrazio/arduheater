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

#ifndef __PRINT_H__
#define __PRINT_H__

namespace serial {
  namespace print {
    inline void string(const char* s) {
      while (*s)
        Serial.write(*s++);
    }

    inline void PGM(const char* s) {
      char c;
      while ((c = pgm_read_byte(s))) {
        Serial.write(c);
        ++s;
      }
    }

    namespace chr {
      inline void eol() {
        serial::print::PGM(string_eol);
      }

      inline void space() {
        serial::print::PGM(string_space);
      }

      inline void comma() {
        serial::print::PGM(string_comma);
      }

      inline void minus() {
        serial::print::PGM(string_minus);
      }

      inline void zero() {
        serial::print::PGM(string_zero);
      }
    }

    void number(uint32_t n, const uint8_t& base, const uint8_t& digits) {
      unsigned char buf[digits];
      uint8_t i = 0;

      for (; i < digits; i++) {
        buf[i] = n % base;
        n /= base;
      }

      for (; i > 0; i--) {
        if (i < digits && base == 2 && !((digits - i) % 4))
          serial::print::chr::space();
        Serial.write('0' + buf[i - 1]);
      }
    }

    inline void number(const uint32_t& n, const uint8_t& base) {
      if (!n) {
        serial::print::chr::zero();
        return;
      }

      uint32_t c = n;
      uint8_t digits = 0;
      while (c) { c /= 10; digits++; }
      serial::print::number(n, base, digits);
    }

    // uint8: 0 to 255
    inline void uint8(const uint8_t& n) {
      uint8_t digits = 3;
      if (n < 10) { digits = 1; }
      else if (n < 100) { digits = 2; }
      serial::print::number(n, 10, digits);
    }

    // int8: -127 to 127
    inline void int8(const int8_t& n) {
      if (n < 0) {
        serial::print::chr::minus();
        serial::print::uint8(-n);
      } else serial::print::uint8(n);
    }

    // uint16: 0 to 65535
    inline void uint16(const uint16_t& n) {
      serial::print::number(n, 10);
    }

    // int16: -32768 to 32767
    inline void int16(const int16_t& n) {
      if (n < 0) {
        serial::print::chr::minus();
        serial::print::uint16(-n);
      } else serial::print::uint16(n);
    }

    // uint32: 0 to 4294967295L
    inline void uint32(const uint32_t& n) {
      serial::print::number(n, 10);
    }

    // int32: -2147483648L to 2147483647L
    inline void int32(const int32_t& n) {
      if (n < 0) {
        serial::print::chr::minus();
        serial::print::uint32(-n);
      } else serial::print::uint32(n);
    }

    void float32(float n, const uint8_t& decimal_places) {
      if (n < 0) {
        serial::print::PGM(PSTR("-"));
        n = -n;
      }

      uint8_t decimals = decimal_places;

      // Quickly convert values expected to be E0 to E-4.
      while (decimals >= 2) {
        n *= 100;
        decimals -= 2;
      }

      if (decimals) { n *= 10; }
      n += 0.5; // Add rounding factor. Ensures carryover through entire value.

      // Generate digits backwards and store in string.
      unsigned char buf[10];
      uint8_t i = 0;
      uint32_t a = (long)n;

      // Place decimal point, even if decimal places are zero.
      buf[decimal_places] = '.';

      while(a > 0) {
        if (i == decimal_places) { i++; } // Skip decimal point location
        buf[i++] = (a % 10) + '0'; // Get digit
        a /= 10;
      }

      // Fill in zeros to decimal point for (n < 1)
      while (i < decimal_places)
        buf[i++] = '0';

      // Fill in leading zero, if needed.
      if (i == decimal_places) {
        i++;
        buf[i++] = '0';
      }

      // Print the generated string.
      for (; i > 0; i--)
        Serial.write(buf[i-1]);
    }

    namespace base2 {
      inline void uint8(const uint8_t& n) {
        serial::print::number(n, 2, 8);
      }

      inline void int8(const int8_t& n) {
        serial::print::number(n, 2, 8);
      }

      inline void uint16(const uint16_t& n) {
        serial::print::number(n, 2, 16);
      }

      inline void int16(const int16_t& n) {
        serial::print::number(n, 2, 16);
      }

      inline void uint32(const uint32_t& n) {
        serial::print::number(n, 2, 32);
      }

      inline void int32(const int32_t& n) {
        serial::print::number(n, 2, 32);
      }
    }
  }
};

#endif
