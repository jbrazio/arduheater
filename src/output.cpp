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

#include "output.h"
#include <Arduino.h>

/**
 * @brief Global variable initialization
 */
Output::channel_t Output::s_channel[4];

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void Output::update_sensor(const uint8_t &chan, const uint16_t &raw_value)
{
  //
  // TODO: Update thermistor state: connected/disconnected
  // TODO: Disable outputs on error
  //
  if(raw_value < 51 || raw_value > 971)
  {
    s_channel[chan].sensor.set_value(THERMISTOR_ERR_TEMP);
    s_channel[chan].heater.set_connected(false);
    s_channel[chan].heater.stop();
  }

  else {
    s_channel[chan].sensor.set_value(raw_value);
    s_channel[chan].heater.set_connected(true);

    const float t = s_channel[chan].sensor.temp();
    s_channel[chan].heater.eval_pid(t);
  }

  analogWrite(get_heater_pin(chan), s_channel[chan].heater.get_value());
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void Output::update_heater(const uint8_t &chan, const int16_t &setpoint)
{
  s_channel[chan].heater.set_target(setpoint);
}
