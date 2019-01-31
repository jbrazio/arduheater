/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2019 João Brázio [joao@brazio.org]
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

#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include "output.h"
#include "environment.h"

inline void output_callback(const uint8_t& channel, const uint16_t& value) {
  if(&output[channel]) (&output[channel])->callback(channel, value);
}

inline void environment_calback(const float& dew) {
  char buffer[60];

  for(size_t i = 0; i < asizeof(output); i++) {
    if(ambient.is_connected()) {
      // Update the output setpoint as long as it is connected.
      output[i].set_setpoint(dew);
      //if(i == 1 || i == 3) output[i].set_setpoint_offset(10);
      //output[i].set_setpoint(40);

      if(output[i].is_enabled()) {
        // Update the PID so the ISR can update the output power. We do not
        // use the "dew" argument because it needs to be compensated for any
        // special offsets the output may have, .setpoint() will do this for
        // us automatically.
        if(ambient.is_ready()) {
          output[i].eval_pid();
        }

        // This will act as a thermal protection by disabling the output if
        // the ambient sensor data is garbage.
        else {
          sprintf_P(buffer, PSTR("W Output-%i disabled, ambient sensor not ready."), i +1);
          LogLn::string(buffer);
          output[i].disable();
        }
      }
    }

    else {
      // This will act as a thermal protection by disabling the output if
      // the ambient sensor goes away.
      if(output[i].is_enabled()) {
        sprintf_P(buffer, PSTR("W Output-%i disabled, ambient sensor not connected."), i +1);
        LogLn::string(buffer);
        output[i].disable();
      }
    }
  }
}

#endif
