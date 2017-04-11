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
    case 0:
      if (buffer[1] != 0x00) {
        result(REPLY_INVALID_SYNTAX);
      } else { status(); }
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

     case '$':
      switch (buffer[1]) {
        case 0:
          help();
          break;

        case '$':
          if (buffer[3] != 0x00) {
            result(REPLY_INVALID_SYNTAX);
          } else { settings(); }
          break;

        case 's':
          if (buffer[3] != 0x00) {
            result(REPLY_INVALID_SYNTAX);
          } else { save(); }
          break;

        case 'i':
          if (buffer[3] != 0x00) {
            result(REPLY_INVALID_SYNTAX);
          } else { buildinfo(); }
          break;

        case 'd':
          if (buffer[3] != 0x00) {
            result(REPLY_INVALID_SYNTAX);
          } else { eeprom::defaults(); save(); }
          break;

        case 'r':
          if (buffer[3] != 0x00) {
            result(REPLY_INVALID_SYNTAX);
          } else { registers(); }
          break;

        default:
          char* str = (char*) buffer;

          // we use strtod with a typecast instead of strtol because the AVR version
          // of strtod will store nullptr at __endptr if the conversion fails.
          // we skip one byte because we already know it's value '$'
          const uint8_t key = (uint8_t) strtod(str +1, &str);
          //serial::print::pair::uint8(PSTR("key"), key);

          if ((key == 0.0F && str == nullptr) || *str != '=') {
            result(REPLY_INVALID_SYNTAX);
            break;
          }

          // we skip one byte because we already know it's value '='
          const float val = strtod(str +1, &str);
          //serial::print::pair::float32(PSTR("val"), val, 2);

          if ((val == 0.0F && str == nullptr) || *str != 0x00) {
            result(REPLY_INVALID_SYNTAX);
            break;
          }

          set(key, val);
          break;
      }
      break;

    default:
      result(REPLY_INVALID_COMMAND);
      break;
  }
}

void cmd::help() {
  serial::println::PGM(PSTR("$$ (view settings)"));
  serial::println::PGM(PSTR("$D (load default settings)"));
  serial::println::PGM(PSTR("$I (view build info)"));
  serial::println::PGM(PSTR("$S (save settings to eeprom)"));
  serial::println::PGM(PSTR("$R (view register status)"));
  serial::println::PGM(PSTR("$key=value (set a setting)"));
  serial::println::PGM(PSTR("? (current status)"));
  serial::println::PGM(PSTR("+x (enable output x)"));
  serial::println::PGM(PSTR("-x (disable output x)"));
  serial::println::PGM(PSTR("#x (autotune output x)"));
  //serial::println::PGM(PSTR(""));
}

void cmd::buildinfo() {
  serial::println::PGM(PSTR("Arduheater " ARDUHEATER_VERSION " build " ARDUHEATER_VERSION_BUILD));
  serial::println::PGM(PSTR("Build date: " __DATE__ " " __TIME__));

  #ifdef PLATFORMIO
    serial::print::PGM(PSTR("Framework: Platformio "));
  #else
    serial::print::PGM(PSTR("Framework: "));
  #endif

  #ifdef ARDUINO
    serial::println::PGM(PSTR("Arduino"));
  #else
    serial::println::PGM(PSTR("Atmel AVR (native)"));
  #endif

  #ifdef ARDUINO_AVR_UNO
    serial::println::PGM(PSTR("Board: Arduino UNO"));
  #endif

  #ifdef ARDUINO_AVR_NANO
    serial::println::PGM(PSTR("Board: Arduino Nano"));
  #endif

  #ifdef ARDUINO_AVR_PRO
    serial::println::PGM(PSTR("Board: Arduino PRO Mini"));
  #endif

    serial::print::PGM(PSTR("Number of outputs: "));
    serial::println::uint8(NUM_OUTPUTS);
}

void cmd::status() {
  serial::print::PGM(PSTR(":"));

  if (sys.sensor & AMBIENT_SENSOR_READY) {
    const float t = amb.t() + amb.config.t_offset;
    const float h = amb.rh() + amb.config.rh_offset;
    const float d = utils::dew(t, h) + amb.config.dew_offset;

    serial::print::PGM(PSTR("amb[t:"));
    serial::print::float32(t, 2);
    serial::print::PGM(PSTR("C h:"));
    serial::print::float32(h, 2);
    serial::print::PGM(PSTR("% d:"));
    serial::print::float32(d, 2);
    serial::println::PGM(PSTR("C]"));
  } else { serial::println::PGM(PSTR("amb[sensor error]")); }

  for (size_t channel = 0; channel < NUM_OUTPUTS; channel++) {
    if (ntc.is_ready(channel)) {
      serial::print::PGM(PSTR(":out"));
      serial::print::uint8(channel);
      serial::print::PGM(PSTR("[t:"));
      serial::print::float32(ntc.t(channel), 2);
      serial::print::PGM(PSTR("C raw:"));
      serial::print::int16(ntc.raw(channel));
      serial::print::PGM(PSTR(" pwr:"));
      serial::print::PGM((sys.output & (OUTPUT0_ENABLED << channel))
        ? PSTR("on") : PSTR("off"));
      serial::print::PGM(PSTR(" set:"));
      serial::print::float32(out[channel].alg.setpoint(), 2);
      serial::print::PGM(PSTR("C pwm:"));
      serial::print::uint8((sys.output & (OUTPUT0_ENABLED << channel))
        ? out[channel].alg.output() : 0);
      serial::print::PGM(PSTR(" Kp:"));
      serial::print::float32(out[channel].alg.Kp(), 2);
      serial::print::PGM(PSTR(" Ki:"));
      serial::print::float32(out[channel].alg.Ki(), 2);
      serial::print::PGM(PSTR(" Kd:"));
      serial::print::float32(out[channel].alg.Kd(), 2);
      serial::println::PGM(PSTR("]"));
    }
  }
}

void cmd::registers() {
  serial::print::PGM(PSTR(":"));

  serial::print::PGM(PSTR("system[state:"));
  serial::print::base2::uint8((const uint8_t) sys.state);
  serial::print::PGM(PSTR(", status:"));
  serial::print::base2::uint8((const uint8_t) sys.sensor);
  serial::print::PGM(PSTR(", output:"));
  serial::print::base2::uint8((const uint8_t) sys.output);
  serial::println::PGM(PSTR("]"));
}

void cmd::result(const uint8_t& code) {
  if (code == REPLY_OK) {
    serial::println::PGM(PSTR("ok"));
    return;
  }

  serial::print::PGM(PSTR("err: "));

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
      serial::println::PGM(PSTR("output is already enabled"));
      break;

    case REPLY_OUTPUT_INACTIVE:
      serial::println::PGM(PSTR("output is already disabled"));
      break;

    case REPLY_OUTPUT_OUTBOUNDS:
      serial::println::PGM(PSTR("output id out of bounds"));
      break;
  }
}

void cmd::enableheater(const char& c) {
  const uint8_t id = (c >= 0x20 && c <= 0x39) ? c - '0' : c;

  if (id >= NUM_OUTPUTS) {
    result(REPLY_OUTPUT_OUTBOUNDS);
    return;
  }

  if (ntc.is_ready(id)) {
    // only allow enable commands to be sent to outputs
    // where the ntc is reporting correct data
    if (sys.output & (OUTPUT0_ENABLED << id)) {
      // if the output is already active warn the user
      // and exit the command
      result(REPLY_OUTPUT_ACTIVE);

    } else {
      sys.output |= (OUTPUT0_ENABLED << id);
      out[id].alg.start();

      // only reply OK if this function is called from serial
      if (c >= 0x20 && c <= 0x39) result(REPLY_OK);
    }

  } else { result(REPLY_NTC_NOT_READY); }
}

void cmd::disableheater(const char& c) {
  const uint8_t id = (c >= 0x20 && c <= 0x39) ? c - '0' : c;

  if (id >= NUM_OUTPUTS) {
    result(REPLY_OUTPUT_OUTBOUNDS);
    return;
  }

  if (ntc.is_ready(id)) {
    // only allow enable commands to be sent to outputs
    // where the ntc is reporting correct data
    if (sys.output & (OUTPUT0_ENABLED << id)) {
      sys.output &= ~(OUTPUT0_ENABLED << id);
      out[id].alg.stop();

      // only reply OK if this function is called from serial
      if (c >= 0x20 && c <= 0x39) result(REPLY_OK);

    } else {
      // if the output is already inactive warn the user
      // and exit the command
      result(REPLY_OUTPUT_INACTIVE);
    }

  } else { result(REPLY_NTC_NOT_READY); }
}

void cmd::autotune(const char& c) {
  const uint8_t id = c - '0';

    if (id >= NUM_OUTPUTS) {
      result(REPLY_OUTPUT_OUTBOUNDS);
      return;
    }

    if (ntc.is_ready(id)) {
      out[id].alg.autotune();

    } else { result(REPLY_NTC_NOT_READY); }
}

void cmd::save() {
  eeprom::save();
  result(REPLY_OK);
}

void cmd::set(const uint8_t& key, const float& val) {
  switch (key) {
    // Ambient sensor ---------------------------------------------------------
    case 1:
      amb.config.t_offset = val;
      break;

    case 2:
      amb.config.rh_offset = val;
      break;

    case 3:
      amb.config.dew_offset = val;
      break;

    // Heater #0 --------------------------------------------------------------
    #if (NUM_OUTPUTS > 0)
      case 4:
        out[0].config.autostart = (bool) val;
        break;

      case 5:
        out[0].config.offset = val;
        break;

      case 6:
        out[0].config.min = (uint8_t) val;
        break;

      case 7:
        out[0].config.max = (uint8_t) val;
        break;

      case 8:
        out[0].config.Kp = val;
        break;

      case 9:
        out[0].config.Ki = val;
        break;

      case 10:
        out[0].config.Kd = val;
        break;
    #endif

    // Heater #1 --------------------------------------------------------------
    #if (NUM_OUTPUTS > 1)
      case 11:
        out[1].config.autostart = (bool) val;
        break;

      case 12:
        out[1].config.offset = val;
        break;

      case 13:
        out[1].config.min = (uint8_t) val;
        break;

      case 14:
        out[1].config.max = (uint8_t) val;
        break;

      case 15:
        out[1].config.Kp = val;
        break;

      case 16:
        out[1].config.Ki = val;
        break;

      case 17:
        out[1].config.Kd = val;
        break;
    #endif

    // Heater #2 --------------------------------------------------------------
    #if (NUM_OUTPUTS > 2)
      case 18:
        out[2].config.autostart = (bool) val;
        break;

      case 19:
        out[2].config.offset = val;
        break;

      case 20:
        out[2].config.min = (uint8_t) val;
        break;

      case 21:
        out[2].config.max = (uint8_t) val;
        break;

      case 22:
        out[2].config.Kp = val;
        break;

      case 23:
        out[2].config.Ki = val;
        break;

      case 24:
        out[2].config.Kd = val;
        break;
    #endif

    // Heater #3 --------------------------------------------------------------
    #if (NUM_OUTPUTS > 3)
      case 25:
        out[3].config.autostart = (bool) val;
        break;

      case 26:
        out[3].config.offset = val;
        break;

      case 27:
        out[3].config.min = (uint8_t) val;
        break;

      case 28:
        out[3].config.max = (uint8_t) val;
        break;

      case 29:
        out[3].config.Kp = val;
        break;

      case 30:
        out[3].config.Ki = val;
        break;

      case 31:
        out[3].config.Kd = val;
        break;
    #endif
  }

  save();
  eeprom::sync();
}

void cmd::settings() {
  #ifndef print_setting_uint8
    #define print_setting_uint8(id, key, desc) do { \
      serial::print::PGM(PSTR("$" #id "=")); \
      serial::print::uint8(key); \
      serial::println::PGM(PSTR(" (" #desc ")")); } while(0)
  #endif

  #ifndef print_setting_float32
    #define print_setting_float32(id, key, desc) do { \
      serial::print::PGM(PSTR("$" #id "=")); \
      serial::print::float32(key, 2); \
      serial::println::PGM(PSTR(" (" #desc ")")); } while(0)
  #endif

  print_setting_float32(1,  amb.config.t_offset,     ambient temperature offset);
  print_setting_float32(2,  amb.config.rh_offset,    ambient relative humidity offset);
  print_setting_float32(3,  amb.config.dew_offset,   ambient dew offset);

  #if (NUM_OUTPUTS > 0)
    serial::print::chr::eol();
    print_setting_uint8  (4,  out[0].config.autostart, heater #0 autostart (bool));
    print_setting_float32(5,  out[0].config.offset,    heater #0 temperature offset);
    print_setting_uint8  (6,  out[0].config.min,       heater #0 PID min value);
    print_setting_uint8  (7,  out[0].config.max,       heater #0 PID max value);
    print_setting_float32(8,  out[0].config.Kp,        heater #0 PID Kp);
    print_setting_float32(9,  out[0].config.Ki,        heater #0 PID Ki);
    print_setting_float32(10, out[0].config.Kd,        heater #0 PID Kd);
  #endif

  #if (NUM_OUTPUTS > 1)
    serial::print::chr::eol();
    print_setting_uint8  (11, out[1].config.autostart, heater #1 autostart (bool));
    print_setting_float32(12, out[1].config.offset,    heater #1 temperature offset);
    print_setting_uint8  (13, out[1].config.min,       heater #1 PID min value);
    print_setting_uint8  (14, out[1].config.max,       heater #1 PID max value);
    print_setting_float32(15, out[1].config.Kp,        heater #1 PID Kp);
    print_setting_float32(16, out[1].config.Ki,        heater #1 PID Ki);
    print_setting_float32(17, out[1].config.Kd,        heater #1 PID Kd);
  #endif

  #if (NUM_OUTPUTS > 2)
    serial::print::chr::eol();
    print_setting_uint8  (18, out[2].config.autostart, heater #2 autostart (bool));
    print_setting_float32(19, out[2].config.offset,    heater #2 temperature offset);
    print_setting_uint8  (20, out[2].config.min,       heater #2 PID min value);
    print_setting_uint8  (21, out[2].config.max,       heater #2 PID max value);
    print_setting_float32(22, out[2].config.Kp,        heater #2 PID Kp);
    print_setting_float32(23, out[2].config.Ki,        heater #2 PID Ki);
    print_setting_float32(24, out[2].config.Kd,        heater #2 PID Kd);
  #endif

  #if (NUM_OUTPUTS > 3)
    serial::print::chr::eol();
    print_setting_uint8  (25, out[3].config.autostart, heater #3 autostart (bool));
    print_setting_float32(26, out[3].config.offset,    heater #3 temperature offset);
    print_setting_uint8  (27, out[3].config.min,       heater #3 PID min value);
    print_setting_uint8  (28, out[3].config.max,       heater #3 PID max value);
    print_setting_float32(29, out[3].config.Kp,        heater #3 PID Kp);
    print_setting_float32(30, out[3].config.Ki,        heater #3 PID Ki);
    print_setting_float32(31, out[3].config.Kd,        heater #3 PID Kd);
  #endif
}
