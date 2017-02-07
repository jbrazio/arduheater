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

#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

class Key : public Observer<bool>
{
private:
  uint8_t m_pin;
  volatile key_state_t m_state;

public:
  explicit Key(const uint8_t& pin)
  {
    m_pin = pin;
    m_state = KEY_RELEASED;
    pinMode(pin, INPUT_PULLUP);
    attachPinChangeInterrupt();
  }

  ~Key()
  {
    detachPinChangeInterrupt();
  }

  const uint8_t& pin()
  {
    return m_pin;
  }

  void update(const bool& val)
  {
    m_state = (val) ? KEY_PRESSED : KEY_RELEASED;
    serial_print_PGM(PSTR("m_pin("));
    serial_print_uint8_base10(m_pin);
    serial_print_PGM(PSTR("): "));
    serial_print_uint8_base10(m_state);
    SERIAL_EOL;
  }

private:
  void attachPinChangeInterrupt()
  {
    cli();
    // Set the pin as an input
    PCIFR |= bit (digitalPinToPCICRbit(m_pin)); // clear any outstanding interrupt
    PCICR |= bit (digitalPinToPCICRbit(m_pin)); // enable interrupt for the group
    *digitalPinToPCMSK(m_pin) |= bit (digitalPinToPCMSKbit(m_pin));  // enable pin
    sei();
  }

  void detachPinChangeInterrupt()
  {
    if (*digitalPinToPCMSK(m_pin) == 0)
      PCICR &= ~bit (digitalPinToPCICRbit(m_pin));
  }
};

#endif
