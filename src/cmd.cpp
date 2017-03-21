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

void cmd::process(const char* buffer) {
  switch (buffer[0]) {
    case '$':
      switch (buffer[1]) {
        case 0:
          help();
          break;

        case '$':
          if (buffer[3] != 0x00) {
            result(REPLY_INVALID_SYNTAX);
          } else { /* do nothing */ }
          break;


        case 'i':
          if (buffer[3] != 0x00) {
            result(REPLY_INVALID_SYNTAX);
          } else { buildinfo(); }
          break;

        default:
          result(REPLY_INVALID_SYNTAX);
      }
      break;

    case '?':
      if (buffer[2] != 0x00) {
        result(REPLY_INVALID_SYNTAX);
      } else { status(); }
      break;

    case '+':
      if (buffer[2] != 0x00) {
        result(REPLY_INVALID_SYNTAX);
      } else { enableheater(buffer[1]); }
      break;

    case '-':
      if (buffer[2] != 0x00) {
        result(REPLY_INVALID_SYNTAX);
      } else { disableheater(buffer[1]); }
      break;

    case '#':
      if (buffer[2] != 0x00) {
        result(REPLY_INVALID_SYNTAX);
      } else { autotune(buffer[1]); }
      break;

    default:
      result(REPLY_INVALID_COMMAND);
      break;
  }
}

void cmd::help() {
  serial::println::PGM(PSTR("$$ (view Arduheater settings)"));
  serial::println::PGM(PSTR("$i (view build info)"));
  serial::println::PGM(PSTR("? (current status)"));
  //serial::println::PGM(PSTR(""));
}

void cmd::buildinfo() {
  serial::println::PGM(PSTR("Arduheater " ARDUHEATER_VERSION "." ARDUHEATER_VERSION_BUILD));
  serial::println::PGM(PSTR("Build date: " __DATE__ " " __TIME__));
}

void cmd::status() {
  serial::print::PGM(PSTR(":"));

  if (sys.status & STATUS_AMBIENT_READY) {
    serial::print::PGM(PSTR("amb[t:"));
    serial::print::float32(amb.t(), 2);
    serial::print::PGM(PSTR(" h: "));
    serial::print::float32(amb.rh(), 2);
    serial::print::PGM(PSTR("% d:"));
    serial::print::float32(utils::dew(amb.t(), amb.rh()), 2);
    serial::println::PGM(PSTR("C]"));
  } else { serial::println::PGM(PSTR("amb[sensor error]")); }

  for (size_t i = 0; i < NUM_OUTPUTS; i++) {
    if (sys.status & (STATUS_NTC0_READY << i)) {
      serial::print::PGM(PSTR(":out"));
      serial::print::uint8(i);
      serial::print::PGM(PSTR("[t:"));
      serial::print::float32(ntc.t(i), 2);
      serial::print::PGM(PSTR("C pwr:"));
      serial::print::PGM((sys.status & (STATUS_OUT0_ENABLED << i)) ? PSTR("on") : PSTR("off"));
      serial::print::PGM(PSTR(" set:"));
      serial::print::uint8(out[i].alg.setpoint());
      serial::print::PGM(PSTR("C pwm:"));
      serial::print::uint8(out[i].alg.output());
      serial::print::PGM(PSTR(" Kp:"));
      serial::print::float32(out[i].alg.Kp(), 2);
      serial::print::PGM(PSTR("C Ki:"));
      serial::print::float32(out[i].alg.Ki(), 2);
      serial::print::PGM(PSTR("C Kd:"));
      serial::print::float32(out[i].alg.Kd(), 2);
      serial::println::PGM(PSTR("]"));
    }
  }
}

void cmd::result(const uint8_t& code) {
  if (code == REPLY_OK) {
    serial::println::PGM(PSTR(":ok"));
    return;
  }

  serial::print::PGM(PSTR(":err: "));

  switch (code) {
    case REPLY_INVALID_COMMAND:
      serial::println::PGM(PSTR("invalid command"));
      break;

    case REPLY_INVALID_SYNTAX:
      serial::println::PGM(PSTR("invalid syntax"));
      break;

    case REPLY_NTC_NOT_READY:
      serial::println::PGM(PSTR("temp reading error"));
      break;

    case REPLY_OUTPUT_ACTIVE:
      serial::println::PGM(PSTR("output is alraedy on"));
      break;

    case REPLY_OUTPUT_INACTIVE:
      serial::println::PGM(PSTR("output is already off"));
      break;

    case REPLY_OUTPUT_OUTBOUNDS:
      serial::println::PGM(PSTR("output id out of bounds"));
      break;
  }
}

void cmd::enableheater(const char& c) {
    const uint8_t id = c - '0';

    if (id > NUM_OUTPUTS) {
      result(REPLY_OUTPUT_OUTBOUNDS);
      return;
    }

    if (sys.status & (STATUS_NTC0_READY << id)) {
      // only allow enable commands to be sent to outputs
      // where the ntc is reporting correct data
      if (sys.status & (STATUS_OUT0_ENABLED << id)) {
        // if the output is already active warn the user
        // and exit the command
        result(REPLY_OUTPUT_ACTIVE);
      } else {
        sys.status |= (STATUS_OUT0_ENABLED << id);
        out[id].alg.tune(40.48, 17.86, 6.70);
        //out[id].alg.limit(0, 100);
        out[id].alg.setpoint(30);
        out[id].alg.start();
        result(REPLY_OK);
      }
    } else { result(REPLY_NTC_NOT_READY); }
}

void cmd::disableheater(const char& c) {
    const uint8_t id = c - '0';

    if (id > NUM_OUTPUTS) {
      result(REPLY_OUTPUT_OUTBOUNDS);
      return;
    }

    if (sys.status & (STATUS_NTC0_READY << id)) {
      // only allow enable commands to be sent to outputs
      // where the ntc is reporting correct data
      if (sys.status & (STATUS_OUT0_ENABLED << id)) {
        sys.status &= ~(STATUS_OUT0_ENABLED << id);
        out[id].alg.stop();
        result(REPLY_OK);
      } else {
        // if the output is already inactive warn the user
        // and exit the command
        result(REPLY_OUTPUT_INACTIVE);
      }
    } else { result(REPLY_NTC_NOT_READY); }
}

void cmd::autotune(const char& c) {
  const uint8_t id = c - '0';

    if (id > NUM_OUTPUTS) {
      result(REPLY_OUTPUT_OUTBOUNDS);
      return;
    }

    if (sys.status & (STATUS_NTC0_READY << id)) {
      // only allow autotune commands to be sent to outputs
      // where the ntc is reporting correct data
      if (sys.status & (STATUS_OUT0_ENABLED << id)) {
        // do not start the autotune on a heater which is
        // currently active
        result(REPLY_OUTPUT_ACTIVE);
      } else {
        out[id].alg.autotune(id, 25, 10);
      }
    } else { result(REPLY_NTC_NOT_READY); }
}
