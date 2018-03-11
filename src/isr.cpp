/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2018 João Brázio [joao@brazio.org]
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

#include "isr.h"

/**
 * @brief Local variable initialization
 */
volatile uint32_t timer0_overflow_count = 0;


/**
 * @brief Timer0 Overflow handler
 * @details
 *
 */
ISR(TIMER0_OVF_vect)
{
  ++timer0_overflow_count;
}

/**
 * @brief Timer1 Compare Match handler
 * @details At 200Hz ISR each block of four analog and one ambient readings will take 30ms to
 * complete. The overall expected update frequency is 33 blocks per second.
 *
 */
ISR(TIMER1_COMPA_vect)
{
  SCOPE_DEBUG_OUTPUT(5);

  static uint8_t state = 255;
  ++state %= 6;

  switch (state)
  {
    case 0:
    case 1:
    case 2:
    case 3: // read thermistor
      Analog::read(get_sensor_pin(state), &Output::update_sensor_callback);
      break;

    case 4: // read ambient
      Environment::read(&Output::update_ambient_callback);
      break;

    default:
      ;
  }

  SCOPE_DEBUG_OUTPUT(5);
}

/**
 * @brief External Interrupt Request Vector
 * @details PORT D, PD2, INT0
 */
ISR (INT0_vect)
{
  SCOPE_DEBUG_OUTPUT(4);
  Environment::isr();
  SCOPE_DEBUG_OUTPUT(4);
}
