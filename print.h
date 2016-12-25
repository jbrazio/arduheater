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

#ifndef __SPRINT_H__
#define __SPRINT_H__

#include "main.h"

void serial_print(const char* s)
{
  while (*s)
    Serial.write(*s++);
}

void serial_print_PGM(const char* s)
{
  char c;
  while ((c = pgm_read_byte(s))) {
    Serial.write(c);
    ++s;
  }
}

void serial_print_uint8(uint8_t n, uint8_t base, uint8_t digits)
{
  unsigned char buf[digits];
  uint8_t i = 0;

  for (; i < digits; i++)
  {
    buf[i] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    Serial.write('0' + buf[i - 1]);
}

void serial_print_uint8_base2(uint8_t n)
{
  serial_print_uint8(n, 2, 8);
}

void serial_print_uint8_base10(uint8_t n)
{
  uint8_t digits;
  if (n < 10) { digits = 1; }
  else if (n < 100) { digits = 2; }
  else { digits = 3; }
  serial_print_uint8(n, 10, digits);
}

void serial_print_uint32_base10(uint32_t n)
{
  if (n == 0) {
    Serial.write('0');
    return;
  }

  unsigned char buf[10];
  uint8_t i = 0;

  while (n > 0)
  {
    buf[i++] = n % 10;
    n /= 10;
  }

  for (; i > 0; i--)
    Serial.write('0' + buf[i-1]);
}

void serial_print_int32(long n)
{
  if (n < 0) {
    Serial.write('-');
    serial_print_uint32_base10(-n);
    return;
  }
  serial_print_uint32_base10(n);
}

void printFloat(float n, uint8_t decimal_places)
{
  if (n < 0)
  {
    Serial.write('-');
    n = -n;
  }

  uint8_t decimals = decimal_places;

  // Quickly convert values expected to be E0 to E-4.
  while (decimals >= 2)
  {
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

  while(a > 0)
  {
    if (i == decimal_places) { i++; } // Skip decimal point location
    buf[i++] = (a % 10) + '0'; // Get digit
    a /= 10;
  }

  // Fill in zeros to decimal point for (n < 1)
  while (i < decimal_places)
    buf[i++] = '0';

  // Fill in leading zero, if needed.
  if (i == decimal_places)
  {
    i++;
    buf[i++] = '0';
  }

  // Print the generated string.
  for (; i > 0; i--)
    Serial.write(buf[i-1]);
}

#endif
