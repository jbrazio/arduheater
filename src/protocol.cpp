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

#include "protocol.h"

void protocol::process(const char *cmd) {
  char buffer[64];
  Output::runtime_t dump;

  switch (cmd[0]) {
    case 0:
      LogLn::PGM(PSTR("Arduheater " ARDUHEATER_VERSION " ['$' for help]"));
      break;

    case '$':
      LogLn::PGM(PSTR("Currently there is no support for a human on the serial line."));
      break;

    case '+':
      if((uint8_t)(cmd[1] - '0') > 3) { return; }
      output[cmd[1] - '0'].enable();
      break;

    case '-':
      if((uint8_t)(cmd[1] - '0') > 3) { return; }
      output[cmd[1] - '0'].disable();
      break;

    case '?': // Output status --------------------------------------------------------------------
      sprintf_P(buffer, PSTR(":?%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i#"),
        output[0].is_connected(),
        output[1].is_connected(),
        output[2].is_connected(),
        output[3].is_connected(),

        output[0].is_ready(),
        output[1].is_ready(),
        output[2].is_ready(),
        output[3].is_ready(),

        output[0].is_enabled(),
        output[1].is_enabled(),
        output[2].is_enabled(),
        output[3].is_enabled()
      ); Log::string(buffer);

      break;

    case 'A': // Ambient --------------------------------------------------------------------------
      sprintf_P(buffer, PSTR(":A%i,%i,%i#"),
        (int16_t)(ambient.temperature() * 10),
        (int16_t)(ambient.humidity()    * 10),
        (int16_t)(ambient.dew_point()   * 10)
      ); Log::string(buffer);
      break;

    case 'B': // Output data ----------------------------------------------------------------------
      if((uint8_t)(cmd[1] - '0') > 3) { return; }

      sprintf_P(buffer, PSTR(":B%i,%i,%i,%i#"), (uint8_t)(cmd[1] - '0'),
        (int16_t)(output[(uint8_t)(cmd[1] - '0')].temperature() * 10),
        (int16_t)(output[(uint8_t)(cmd[1] - '0')].setpoint()    * 10),
        (uint8_t)(output[(uint8_t)(cmd[1] - '0')].output_value())
      ); Log::string(buffer);
      break;

    case 'C': // PID data -------------------------------------------------------------------------
      if((uint8_t)(cmd[1] - '0') > 3) { return; }
      dump = output[(uint8_t)(cmd[1] - '0')].export_runtime();

      sprintf_P(buffer, PSTR(":C%i,%i,%i,%i,%i#"), (uint8_t)(cmd[1] - '0'),
        (int16_t)(dump.Pterm * 10),
        (int16_t)(dump.Iterm * 10),
        (int16_t)(dump.Dterm * 10),
        (int16_t)(dump.u)
      ); Log::string(buffer);
      break;

    case 'D': // Output config --------------------------------------------------------------------
              // TODO
              // this section needs to be better structured
              //
      if((uint8_t)(cmd[1] - '0') > 3) { return; }

      if(strlen(cmd) < 3) {
        sprintf_P(buffer, PSTR(":D%i,%i,%i,%i,%i,%i,%i,%i,%i#"), (uint8_t)(cmd[1] - '0'),
          (uint8_t)(output[(uint8_t)(cmd[1] - '0')].min_output()),
          (uint8_t)(output[(uint8_t)(cmd[1] - '0')].max_output()),
          (uint8_t)(output[(uint8_t)(cmd[1] - '0')].is_autostart()),
          (int16_t)(output[(int16_t)(cmd[1] - '0')].temp_offset()     * 10),
          (int16_t)(output[(int16_t)(cmd[1] - '0')].setpoint_offset() * 10),
          (int16_t)(output[(int16_t)(cmd[1] - '0')].kp() * 10),
          (int16_t)(output[(int16_t)(cmd[1] - '0')].ki() * 10),
          (int16_t)(output[(int16_t)(cmd[1] - '0')].kd() * 10)
        ); Log::string(buffer);
      }

      else {
        memset(&buffer, 0, sizeof(buffer));
        int16_t args[9] = { 0 };
        int i = 0, n = 0;

        while (*cmd)
        {
          char c = *cmd++;

          switch(c)
          {
            case ',':
              args[n++] = atol2(buffer);
              memset(&buffer, 0, sizeof(buffer));
              i = 0;
              break;

            default:
              if(is_digit(c)) { buffer[i++] = c; }
          }
        }

        // last one is missed due to the string not
        // having the last ',' separator.
        args[n] = atol2(buffer);

        Output::config_t new_config = {
          (uint8_t)args[1], (uint8_t)args[2], (bool)args[3], (float)(args[4] * 0.1F),
          (float)(args[5] * 0.1F), (float)(args[6] * 0.1F), (float)(args[7] * 0.1F), (float)(args[8] * 0.1F)
        };
/*
        //LogLn::number(args[0]);
        LogLn::number(new_config.min);
        LogLn::number(new_config.max);
        LogLn::number(new_config.autostart);
        LogLn::number(new_config.temp_offset);
        LogLn::number(new_config.setpoint_offset);
        LogLn::number(new_config.Kp);
        LogLn::number(new_config.Ki);
        LogLn::number(new_config.Kd);
        Log::eol();
*/
        output[args[0]].import_config(new_config);
      }
      break;

    case 'F': // Sensor config --------------------------------------------------------------------
      if((uint8_t)(cmd[1] - '0') > 3) { return; }

      sprintf_P(buffer, PSTR(":D%i,%i,%i,%i,%i#"), (uint8_t)(cmd[1] - '0'),
        (int16_t)(output[(uint8_t)(cmd[1] - '0')].nominal_temp()   * 10),
        (int16_t)(output[(uint8_t)(cmd[1] - '0')].resistor_value() * 10),
        (int16_t)(output[(uint8_t)(cmd[1] - '0')].bcoefficient_value()),
        (int16_t)(output[(uint8_t)(cmd[1] - '0')].nominal_value())
      ); Log::string(buffer);
      break;

    case 'V': // Version --------------------------------------------------------------------------
      sprintf_P(buffer, PSTR(":V%S#"),
        PSTR(ARDUHEATER_VERSION)
      ); Log::string(buffer);
      break;


    case 'I': // Reserved -------------------------------------------------------------------------
    case 'E':
    case 'W':
      // These are reserved keywords for the status messages.
      break;

    default: // Default ---------------------------------------------------------------------------
      LogLn::PGM(PSTR("E Invalid command, type '$' for help."));
      break;
  }
}
