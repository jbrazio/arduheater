/**
 * Arduheater - Heat controller for astronomy usage
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

#include "arduheater.h"

// WARNING: max 12750ms as input
uint8_t utils::msectotick(const uint16_t& ms)
{
  return (ms / HEARTBEAT);
}

float utils::steinhart(uint16_t raw)
{
  if (raw > 1023) { raw = 1023; }

  float steinhart;
  steinhart  = THERMISTOR_SERIESRESISTOR / (1023.0 / raw - 1);  // convert raw to ohms
  steinhart  = steinhart / THERMISTOR_NOMINAL_VAL;              // (R/Ro)
  steinhart  = log(steinhart);                                  // ln(R/Ro)
  steinhart /= THERMISTOR_BCOEFFICIENT;                         // 1/B * ln(R/Ro)
  steinhart += 1.0 / (THERMISTOR_NOMINAL_TEMP + 273.15);        // + (1/To)
  steinhart  = 1.0 / steinhart;                                 // invert
  steinhart -= 273.15;                                          // convert to K to C
  return steinhart;
}
