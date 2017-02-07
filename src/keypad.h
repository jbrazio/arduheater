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

#include <Arduino.h>
#include "singleton.h"
#include "struct.h"
#include "subject.h"

extern volatile uint32_t test_counter;

class keypad : public Subject<message_t>
{
private:
    key_t m_key[2];
    volatile uint8_t v_cache[2];

public:
  typedef Singleton<keypad> single;

  void init() {;}

  void init(const uint8_t& pin_A, const uint8_t& pin_B) {
    m_key[0].pin = pin_A;
    m_key[1].pin = pin_B;

    for (size_t i = 0; i < 2; i++) {
      pinMode(m_key[i].pin, INPUT_PULLUP);
      attachPinChangeInterrupt(m_key[i].pin);
      v_cache[i] = *portInputRegister(i+2);
    }
  }

  void isr(const uint8_t& port)
  {
    const uint8_t index  = port - 2;
    const uint8_t reg    = *portInputRegister(port);
    const uint8_t change = (v_cache[index] ^ reg);

    if (reg == NOT_A_PORT || ! change) { return; }

    for (size_t i = 0; i < 2; i++)
    {
      const uint8_t mask = digitalPinToBitMask(m_key[i].pin);

      // As we are using pull-up resistors on the physical buttons
      // the logic within portInputRegister() will be reversed thus
      // to detect a key released we need to detect a positive bit.
      if ((change & mask) && digitalPinToPort(m_key[i].pin) == port) {
        if (reg & mask) m_key[i].timeout = 2;  // trigger on next work cycle
        else            m_key[i].timeout = 11; // wait for 1000ms
      }
    }

    v_cache[index] = reg;
  }

  __attribute__((always_inline))
  void worker() {
    for (size_t i = 0; i < 2; i++) {
      if (m_key[i].timeout) {
        --m_key[i].timeout;
        if (m_key[i].timeout < 2) {
          const uint8_t port = digitalPinToPort(m_key[i].pin);
          const uint8_t reg  = *portInputRegister(port);
          const uint8_t mask = digitalPinToBitMask(m_key[i].pin);

          if (m_key[i].state == KEYPRESS_LONG && (reg & mask)) {
            m_key[i].state = KEYPRESS_UNKNOWN;
            m_key[i].timeout = 0;
            return;
          }

          if (reg & mask) m_key[i].state = KEYPRESS_SHORT;
          else            m_key[i].state = KEYPRESS_LONG;

          m_key[i].timeout = 0;

          key_type_t button = KEY_TYPE_UNKNOWN;
          if (i == 0) button = KEY_TYPE_LEFT;
          else if (i == 1) button = KEY_TYPE_RIGHT;

          message_t payload;
          payload.type = MESSAGE_TYPE_BUTTON;
          payload.data.ub[0] = button;
          payload.data.ub[1] = m_key[i].state;

          notify(payload);

          if (m_key[i].state != KEYPRESS_LONG)
            m_key[i].state = KEYPRESS_UNKNOWN;
        }
      }
    }
  }

protected:
  void attachPinChangeInterrupt(const uint8_t& pin)
  {
    cli();
    PCIFR |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    sei();
  }
};

#endif
