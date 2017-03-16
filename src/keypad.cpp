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


#include "common.h"

keypad::keypad() {
  m_key[0].pin = UI_KEYPAD_A_PIN;
  m_key[1].pin = UI_KEYPAD_B_PIN;

  for (size_t i = 0; i < 2; i++) {
    pinMode(m_key[i].pin, INPUT_PULLUP);
    attachPinChangeInterrupt(m_key[i].pin);
    v_cache[i] = *portInputRegister(i+2);
  }
}

void keypad::isr(const uint8_t& port)
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

void keypad::irq() {
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

        message_t payload;
        payload.category   = MSG_CAT_KEYPAD;
        payload.data[0].dw = (i == 0) ? KEYCODE_LEFT : KEYCODE_RIGHT;
        payload.data[1].dw = m_key[i].state;
        notify(payload);

        if (m_key[i].state != KEYPRESS_LONG) { m_key[i].state = KEYPRESS_UNKNOWN; }
      }
    }
  }
}

void keypad::attachPinChangeInterrupt(const uint8_t& pin)
{
  cli();
  PCIFR |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  sei();
}
