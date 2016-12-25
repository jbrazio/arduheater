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

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "main.h"

class KeyPad : Subject<key_type_t>
{
private:
  Key m_keys[2] = {
    Key(UI_KEYPAD_A_PIN),
    Key(UI_KEYPAD_B_PIN)
  };
  volatile uint8_t v_cache[3]; // Input Pin Register

public:
  typedef Singleton<KeyPad> single;

  KeyPad()
  {
    for (size_t i = 0; i < 3; i++)
      v_cache[i] = *portInputRegister(i+2);
  }

  __attribute__((optimize ("O3")))
  void isr(const uint8_t& port)
  {
    PORTB |= 0x20;

    const uint8_t index  = port - 2;
    const uint8_t now    = *portInputRegister(port);
    const uint8_t change = (v_cache[index] ^ now);

    if (now == NOT_A_PORT || ! change) { return; }

    for (size_t i = 0; i < 2; i++)
    {
      const uint8_t pin    = m_keys[i].pin();
      const uint8_t mask   = digitalPinToBitMask(pin);

      if (digitalPinToPort(pin) == port && (change & mask))
        // Due to pull-up the change logic is reversed
        m_keys[i].update(((now & mask)) ? false : true);
    }

    v_cache[index] = now;
    PORTB ^= 0x20;

    //TODO: Don't create a dependency of "menu", use function pointers instead
    //if (PINB & 0x01) GUI::single::instance().pageUp();
    //if (PINB & 0x10) GUI::single::instance().pageDown();
  }

  __attribute__((always_inline))
  inline void worker()
  {
    ;
  }

  virtual void set(const key_type_t& val)
  {
    ;
  }
};

#endif
