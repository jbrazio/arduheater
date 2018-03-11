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

#include "output.h"

/**
 * @brief Global variable initialization
 */
Output::channel_t Output::s_channel[4];

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void Output::update_ambient_callback(const float &dp)
{
  for(size_t i = 0; i < 4; i++) {
    // Ignore output if it is disconnected
    // TODO: Check if the compiler is smart enough to optimize the following
    //       calls, otherwise update this to use direct var access.
    if (! Output::channel(0).is_connected()) continue;
    //Output::channel(0).setpoint(dp);
    Output::channel(0).setpoint(25);
  }
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void Output::update_sensor_callback(const uint8_t &chan, const uint16_t &raw_value)
{
  //
  // TODO: Update thermistor state: connected/disconnected
  // TODO: Disable outputs on error
  //
  if(raw_value < HEATER_MIN_VAL || raw_value > HEATER_MAX_VAL)
  {
    if(s_channel[chan].sensor.is_connected()) {
      s_channel[chan].sensor.set_connected(false);
      s_channel[chan].heater.set_connected(false);

      Log::PGM(PSTR("INFO: Output #"));
      Log::number(chan +1);
      Log::PGM(PSTR(" disconnected."));
      Log::eol();
    }

    s_channel[chan].sensor.set_value(HEATER_ERR_VAL);
    s_channel[chan].heater.stop();
  }

  else {
    if(! s_channel[chan].sensor.is_connected()) {
      s_channel[chan].sensor.set_connected(true);
      s_channel[chan].heater.set_connected(true);

      // TODO: Autostart heater if option active

      Log::PGM(PSTR("INFO: Output #"));
      Log::number(chan +1);
      Log::PGM(PSTR(" connected."));
      Log::eol();
    }

    s_channel[chan].sensor.set_value(raw_value);

    const float t = s_channel[chan].sensor.temp();
    s_channel[chan].heater.eval_pid(t);
  }

  IO::write(get_heater_pin(chan), s_channel[chan].heater.get_value());
}
