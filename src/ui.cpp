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

ui::ui()
  : m_active_card(nullptr)
{
  show(CARD_BLANK); // clears the screen
}

void ui::update(const message_t& message) {
  switch (message.category) {
    case MSG_UPDATE_LCD: {
      DEBUGPRN("ui::update(): MSG_UPDATE_LCD");
      m_active_card->needs_drawing(true);
      break;
    }

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
    // do time accounting for card's timeout
    if (m_active_card->has_timeout() && m_active_card->did_timeout()) {
      m_active_card->timeout();
      show(m_active_card->timeout_card());
    }

    // do time accounting for card's slideshow
    if (m_active_card->has_pages() && m_active_card->did_page_timeout()) {
      DEBUGPRN("ui::irq(): next_page()");
      m_active_card->next_page();
      m_active_card->needs_drawing(true);
    }

    // refresh the active card
    if (m_active_card->needs_drawing()) {
      DEBUGPRN("ui::irq(): needs_drawing()");
      m_active_card->needs_drawing(false);
      m_active_card->draw();
    }
  }
}

void ui::show(const card_index_t& card_index, const uint16_t& card_timeout) {
  delete m_active_card; // free the active card's memory

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
  m_active_index = card_index;

  // prepare card's timeout
  m_active_card->set_timeout(card_timeout);
}

void ui::process_keypress(const message_t& message) {
  keycode_t  button = static_cast<keycode_t>(message.data[0].dw);
  keypress_t state  = static_cast<keypress_t>(message.data[1].dw);

  serial::print::PGM(PSTR("buton("));
  serial::print::uint8(button);
  serial::print::PGM(PSTR("): "));
  serial::println::uint8(state);

  static bool boo = false;
  boo = ! boo;
  runtime::single::instance().m_output[0].pid.mode(boo ? PID::AUTOMATIC : PID::MANUAL);
  if (!boo) runtime::single::instance().m_output[0].pid.output(0);

  // reset the card's timeout if needed
  if (m_active_card->has_timeout()) { m_active_card->reset_timeout(); }
}
