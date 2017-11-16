/**
 * Arduheater - An intelligent dew buster for astronomy
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

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdint.h>
#include <stdlib.h>

#include <avr/pgmspace.h>

#include "serial.h"

// Special characters
const char string_colon   [] PROGMEM = { 0x3a, 0x00 };
const char string_comma   [] PROGMEM = { 0x2c, 0x00 };
const char string_eol     [] PROGMEM = { 0x0a, 0x00 };
const char string_minus   [] PROGMEM = { 0x2d, 0x00 };
const char string_percent [] PROGMEM = { 0x25, 0x00 };
const char string_space   [] PROGMEM = { 0x20, 0x00 };
const char string_zero    [] PROGMEM = { 0x30, 0x00 };

class Console
{
  /**
   * Disable the creation of an instance of this object.
   * This class should be used as a static class.
   */
  private:
     Console() {;}
    ~Console() {;}

  public:
    static void eol()   { Console::PGM(string_eol); }
    static void space() { Console::PGM(string_space); }
    static void comma() { Console::PGM(string_comma); }
    static void colon() { Console::PGM(string_colon); }
    static void minus() { Console::PGM(string_minus); }
    static void zero()  { Console::PGM(string_zero); }

  public:
    static inline void string(const char* s)
    {
      while (*s)
        Serial::write(*s++);
    }

    static inline void PGM(const char* s)
    {
      char c;
      while ((c = pgm_read_byte(s++)))
        Serial::write(c);
    }

  public:
    // uint8: 0 to 255
    static void number(const uint8_t& n)
    {
      uint8_t digits = 3;
      if (n < 10) { digits = 1; }
      else if (n < 100) { digits = 2; }
      Console::number(n, 10, digits);
    }

    // int8: -127 to 127
    static void number(const int8_t& n)
    {
      if (n < 0) {
        Console::minus();
        Console::number(-n, 10);
      } else Console::number(n);
    }

    // uint16: 0 to 65535
    static void number(const uint16_t& n)
    {
      Console::number(n, 10);
    }

    // int16: -32768 to 32767
    static void number(const int16_t& n)
    {
      if (n < 0) {
        Console::minus();
        Console::number(-n, 10);
      } else Console::number(n, 10);
    }

    // uint32: 0 to 4294967295L
    static void number(const uint32_t& n)
    {
      Console::number(n, 10);
    }

    // int32: -2147483648L to 2147483647L
    static void number(const int32_t& n)
    {
      if (n < 0) {
        Console::minus();
        Console::number(-n, 10);
      } else Console::number(n, 10);
    }

    // float:
    static void number(const float& n)
    {
      Console::fraq(n, 2);
    }
/*
  public:
    static void binary(const uint8_t& n)  { Console::number(n, 2, 8); }
    static void binary(const int8_t& n)   { Console::number(n, 2, 8); }
    static void binary(const uint16_t& n) { Console::number(n, 2, 16); }
    static void binary(const int16_t& n)  { Console::number(n, 2, 16); }
    static void binary(const uint32_t& n) { Console::number(n, 2, 32); }
    static void binary(const int32_t& n)  { Console::number(n, 2, 32); }
*/

  protected:
    static void number(uint32_t n, const uint8_t& base, const uint8_t& digits)
    {
      uint8_t i = 0;
      unsigned char buf[digits];

      for (; i < digits; i++) {
        buf[i] = n % base;
        n /= base;
      }

      for (; i > 0; i--) {
        if (i < digits && base == 2 && !((digits - i) % 4)) Console::space();
        Serial::write('0' + buf[i - 1]);
      }
    }

    static void number(const uint32_t& n, const uint8_t& base)
    {
      if (!n) {
        Console::zero();
        return;
      }

      uint32_t c = n;
      uint8_t digits = 0;
      while (c) { c /= 10; digits++; }
      Console::number(n, base, digits);
    }

    static void fraq(float n, const uint8_t& decimal_places)
    {
      if (n < 0) {
        Console::minus();
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

      // Console the generated string.
      for (; i > 0; i--)
        Serial::write(buf[i-1]);
    }
};

class Consoleln
{
  /**
   * Disable the creation of an instance of this object.
   * This class should be used as a static class.
   */
  private:
     Consoleln() {;}
    ~Consoleln() {;}

  public:
    static inline void string(const char* s) { Console::string(s); Console::eol(); }
    static inline void PGM(const char* s)    { Console::PGM(s); Console::eol();    }

  public:
    static void number(const uint8_t& n)  { Console::number(n); Console::eol(); }
    static void number(const int8_t& n)   { Console::number(n); Console::eol(); }
    static void number(const uint16_t& n) { Console::number(n); Console::eol(); }
    static void number(const int16_t& n)  { Console::number(n); Console::eol(); }
    static void number(const uint32_t& n) { Console::number(n); Console::eol(); }
    static void number(const int32_t& n)  { Console::number(n); Console::eol(); }
    static void number(const float& n)    { Console::number(n); Console::eol(); }
/*
  public:
    static void binary(const uint8_t& n)  { Console::binary(n); Console::eol(); }
    static void binary(const int8_t& n)   { Console::binary(n); Console::eol(); }
    static void binary(const uint16_t& n) { Console::binary(n); Console::eol(); }
    static void binary(const int16_t& n)  { Console::number(n); Console::eol(); }
    static void binary(const uint32_t& n) { Console::binary(n); Console::eol(); }
    static void binary(const int32_t& n)  { Console::binary(n); Console::eol(); }
*/
};

#endif
