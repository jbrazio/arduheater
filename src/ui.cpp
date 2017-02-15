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

#include <Arduino.h>
#include "macros.h"
#include "print.h"
#include "struct.h"
#include "timer1.h"
#include "ui.h"
#include "weather.h"

#include "cards/blank.h"
#include "cards/home.h"
#include "cards/splash.h"

ui::ui()
  : m_active_card(nullptr)
{
  show(CARD_BLANK); // clears the screen
}

void ui::update(const message_t& message) {
  switch (message.category) {
    case MSG_CAT_KEYPAD: {
      DEBUGPRN("ui::update(): MSG_CAT_KEYPAD");
      process_keypress(message);
      break;
    }

    default: { break; }
  }
}

void ui::irq() {
  // this guard will prevent meltdown when the ptr is undef
  if (m_active_card) {
    // allow runtime outsiders to request a lcd refresh
    if (runtime::single::instance().m_lcd_needs_refresh) {
      DEBUGPRN("ui::worker(): m_lcd_needs_refresh");
      runtime::single::instance().m_lcd_needs_refresh = false;
      m_active_card->m_needs_drawing = true;
    }

    // refresh the active card
    if (m_active_card->m_needs_drawing) {
      DEBUGPRN("ui::worker(): m_needs_drawing");
      m_active_card->m_needs_drawing = false;
      m_active_card->draw();
    }

    // do time accounting for card's timeout
    if (m_active_timeout) {
      if (m_active_timeleft > 0) { m_active_timeleft -= HEARTBEAT; }
      else {
        m_active_card->timeout();
        show(m_active_card->m_timeout_card);
      }
    }
  }
}

void ui::show(const card_index_t& card_index, const uint16_t& card_timeout) {
  delete m_active_card; // free the active card's memory
  m_active_index    = card_index; // active card tracker
  m_active_timeout  = card_timeout; // set the card's timeout
  m_active_timeleft = card_timeout; // zero the counter

  // simple card factory
  switch (card_index) {
    case CARD_BLANK: {
      DEBUGPRN("ui::show(): new CardBlank()");
      m_active_card = new CardBlank();
      break;
    }

    case CARD_SPLASH: {
      DEBUGPRN("ui::show(): new CardSplash()");
      m_active_card = new CardSplash();
      break;
    }

    case CARD_HOME: {
      DEBUGPRN("ui::show(): new CardHome()");
      m_active_card = new CardHome();
      break;
    }
  }

  // init() is virtual so, if uneeded, cards may not define it
  m_active_card->init();
}

void ui::process_keypress(const message_t& message) {
  keycode_t  button = static_cast<keycode_t>(message.data[0].dw);
  keypress_t state  = static_cast<keypress_t>(message.data[1].dw);

  serial::print::PGM(PSTR("buton("));
  serial::print::uint8(button);
  serial::print::PGM(PSTR("): "));
  serial::println::uint8(state);

  // reset the card's timeout if needed
  if (m_active_timeout) m_active_timeleft = m_active_timeout;
}
