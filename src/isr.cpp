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

#include "isr.h"

/**
 * @brief Timer1 interrupt handler
 * @details
 *
 */
ISR(TIMER1_COMPA_vect)
{
  static uint8_t state = 255;

  ++state %= 4;
  switch (state)
  {
    case 0:
    case 1:
    case 2:
    case 3:
      Analog::read(get_sensor_pin(state), &Output::update_sensor);
      break;

    case 4:
      // read ambient
      break;

    default:
      ;
  }
}
