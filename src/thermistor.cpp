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

#include "thermistor.h"

/**
 * @brief TODO
 * @details
 *
 */
float Thermistor::temp()
{
  // steinhart
  float temp = constrain(m_value, 1, 1022);
  temp  = m_config.resistor / (1023.0F / temp - 1); // convert raw to ohms
  temp /= m_config.nominalval;                      // (R/Ro)
  temp  = log(temp);                                // ln(R/Ro)
  temp /= m_config.bcoefficient;                    // 1/B * ln(R/Ro)
  temp += 1.0F / (m_config.nominaltemp + 273.15F);  // + (1/To)
  temp  = 1.0F / temp;                              // invert
  temp -= 273.15F;                                  // convert to K to C
  return (roundf(temp * 10) / 10);                  // round to one decimal
}

uint16_t Thermistor::invtemp(const float& temp)
{
  float value = temp;
  value += 273.15F;
  value  = 1.0F / value;
  value -= 1.0F / (m_config.nominaltemp + 273.15F);
  value *= m_config.bcoefficient;
  value  = exp(value);
  value *= m_config.nominalval;
  value  = (1023.0F * value) / (value + m_config.resistor);
  return (uint16_t) value;
}
