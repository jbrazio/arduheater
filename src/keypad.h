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

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "common.h"

class keypad : public Subject<message_t>
{
private:
    key_t m_key[2];
    volatile uint8_t v_cache[2];

public:
  typedef Singleton<keypad> single;

protected:
  void attachPinChangeInterrupt(const uint8_t& pin);

public:
  void init(const uint8_t& pin_A, const uint8_t& pin_B);
  void irq();
  void isr(const uint8_t& port);
};

#endif
