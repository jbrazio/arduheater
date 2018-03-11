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
  switch (cmd[0]) {
    case 0:
      LogLn::PGM(PSTR("Arduheater " ARDUHEATER_VERSION " ['$' for help]"));
      break;

    case '1':
      Output::channel(0).enable();
      break;

    case '2':
      Output::channel(0).disable();
      break;

    case '3':
      //Environment::debug();
      break;

    case '4':
      break;

    default:
      LogLn::PGM(PSTR("ERR: Invalid command"));
      break;
  }
}


/*

HUMAN PROTOCOL

start_char '$'
end_char '\n'

"$i (view build info)"
"$s (view register status)"

"$v (view settings)"
"$r (read settings from eeprom)"
"$w (write settings to eeprom)"
"$d (reset to factory defaults)"

"$key=value (set a setting)"

"$x+ (enable output x)"
"$x- (disable output x)"


MACHINE PROTOCOL

 ------------------- -----------------
| COMMAND           | REPLY           |
|-------------------|-----------------|---------------------------------------------------------------
| 1 2 3 4 5 6 7 8 9 | 1 2 3 4 5 6 7 8 | DESCIPRTION                               |                   |
|-------------------|-----------------|-------------------------------------------|-------------------|
| : G A T #         | : A T X X #     | Get Ambient Temperature                   |                   |
| : G A H #         | : A H X X #     | Get Ambient relative Humidity             |                   |
| : G A D #         | : A D X X #     | Get Ambient Dew point                     |                   |
|-------------------|-----------------|-------------------------------------------|-------------------|
| : G A T O #       | : A T O X X #   | Get Ambient Temperature Offset            |                   |
| : G A H O #       | : A H O X X #   | Get Ambient relative Humidity Offset      |                   |
| : G A D O #       | : A D O X X #   | Get Ambient  Dew point Offset             |                   |
|-------------------|-----------------|-------------------------------------------|-------------------|
| : S A T O X X #   | : A T O X X #   | Set Ambient Temperature Offset            |                   |
| : S A H O X X #   | : A H O X X #   | Set Ambient relative Humidity Offset      |                   |
| : S A D O X X #   | : A D O X X #   | Set Ambient Dew point Offset              |                   |
|-------------------|-----------------|-------------------------------------------|-------------------|
| : G O X T #       | : O X T X X #   | Get the Temperature of Output X           |                   |
| : G O X R T #     | : O X R T X X # | Get the Raw Temperature Value of Output X |                   |
| : G O X S #       | : O X S X X #   | Get the power Status of Output X          | "00" off, "FF" on |
| : G O X P #       | : O X P X X #   | Get the PWM value of Output X             |                   |
| : G O X S P #     | : O X S P X X # | Get the Set Point of Output X             |                   |
|-------------------|-----------------|-------------------------------------------|-------------------|
| : G O X A #       | : O X A X X #   | Get the Autostart value of Output X       | "00" off, "FF" on |
| : G O X T O #     | : O X T O X X # | Get the Temperature Offset of Output X    |                   |
| : G O X S O #     | : O X S O X X # | Get the Setpoint Offset of Output X       |                   |
| : G O X P + #     | : O X P + X X # | Get the Minimum PWM value of Output X     |                   |
| : G O X P - #     | : O X P - X X # | Get the Maximum PWM value of Output X     |                   |
| : G O X K P #     | : O X K P X X # | Get the Kp value of Output X              |                   |
| : G O X K I #     | : O X K I X X # | Get the Ki value of Output X              |                   |
| : G O X K D #     | : O X K D X X # | Get the Kd value of Output X              |                   |
|-------------------|-----------------|-------------------------------------------|-------------------|
| : S O X A X X #   | : O X A X X #   | Set the Autostart value of Output X       | "00" off, "FF" on |
| : S O X T O X X # | : O X T O X X # | Set the Temperature Offset of Output X    |                   |
| : S O X S O X X # | : O X S O X X # | Set the Setpoint Offset of Output X       |                   |
| : S O X P + X X # | : O X P + X X # | Set the Minimum PWM value of Output X     |                   |
| : S O X P - X X # | : O X P - X X # | Set the Maximum PWM value of Output X     |                   |
| : S O X K P X X # | : O X K P X X # | Set the Kp value of Output X              |                   |
| : S O X K I X X # | : O X K I X X # | Set the Ki value of Output X              |                   |
| : S O X K D X X # | : O X K D X X # | Set the Kd value of Output X              |                   |
 ------------------- ----------------- ------------------------------------------- ------------------ |


start_char ':'
end_char '#'
separator_char ','

*/
