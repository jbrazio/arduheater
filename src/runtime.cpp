/**
 * Arduheater - Telescope heat controller
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

#include "common.h"

void runtime::update(const message_t& message) {
  switch (message.category) {
    case MSG_CAT_KEYPAD: {
      DEBUGPRN(3, "runtime::update(): MSG_CAT_KEYPAD");

      #if defined(VERBOSE) && VERBOSE >= 3
      keycode_t  button = static_cast<keycode_t>(message.data[0].dw);
      keypress_t state  = static_cast<keypress_t>(message.data[1].dw);

      serial::print::PGM(PSTR("buton("));
      serial::print::uint8(button);
      serial::print::PGM(PSTR("): "));
      serial::println::uint8(state);
      #endif

      if (runtime::single::instance().m_output[0].running()) {
        runtime::single::instance().m_output[0].off();
      } else {
        runtime::single::instance().m_output[0].on();
      }

      ui::single::instance().update(message);
      break;
    }

    case MSG_CAT_WEATHER: {
      DEBUGPRN(4, "runtime::update(): MSG_CAT_WEATHER");

      #if defined(VERBOSE) && VERBOSE >= 4
      serial::print::PGM(PSTR("MSG_CAT_WEATHER: t: "));
      serial::print::float32(message.data[0].f, 2);
      serial::print::PGM(PSTR(", rh: "));
      serial::print::float32(message.data[1].f, 2);
      serial::print::PGM(PSTR(", dew: "));
      serial::println::float32(utils::weather::dew(message.data[0].f, message.data[1].f), 2);
      #endif

      m_ambient.t   += roundf(message.data[0].f);
      m_ambient.rh  += roundf(message.data[1].f);
      m_ambient.dew += roundf(utils::weather::dew(message.data[0].f, message.data[1].f));

      static int8_t t, rh, dew; // cache values
      if ((m_ambient.t() != t) || (m_ambient.rh() != rh) || (m_ambient.dew() != dew)) {
        t   = m_ambient.t();
        rh  = m_ambient.rh();
        dew = m_ambient.dew();

        ui::single::instance().update({MSG_UPDATE_LCD, 0});
      }
      break;
    }

    case MSG_CAT_NTC: {
      DEBUGPRN(4, "runtime::update(): MSG_CAT_NTC");

      #if defined(VERBOSE) && VERBOSE >= 4
      serial::print::PGM(PSTR("MSG_CAT_NTC: channel: "));
      serial::print::uint8(message.data[0].ub[0]);
      serial::print::PGM(PSTR(", t: "));
      serial::println::float32(message.data[1].f, 2);
      #endif

      uint8_t ch = message.data[0].ub[0];
      m_output[ch].t += roundf(message.data[1].f);

      static int16_t t[4]; // cache values

      //serial::print::pair::int16(PSTR("m_output[ch].t()"), m_output[ch].t());
      //serial::print::pair::int16(PSTR("t[ch]"), t[ch]);

      if (m_output[ch].t() != t[ch]) {
        t[ch] = m_output[ch].t();
        ui::single::instance().update({MSG_UPDATE_LCD, 0});
      }
      break;
    }

    default: { break; }
  }
}
