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
#include "print.h"
#include "struct.h"
#include "timer1.h"
#include "ui.h"

#include "cards/blank.h"
#include "cards/home.h"
#include "cards/splash.h"

ui::ui()
  : m_active_card(nullptr)
{
  serial::println::string("ui::ui()");
  show(CARD_BLANK); // clears the screen
}

void ui::update(const message_t& message) {
  serial::println::string("ui::update()");

  switch(message.type) {
    case MESSAGE_TYPE_UNKNOWN:
      break;

    case MESSAGE_TYPE_BUTTON:
      key_type_t button = static_cast<key_type_t>(message.data.ub[0]);
      keypress_t state  = static_cast<keypress_t>(message.data.ub[1]);

      serial::print::PGM(PSTR("buton("));
      serial::print::uint8(button);
      serial::print::PGM(PSTR("): "));
      serial::println::uint8(state);
      break;
  }
}

void ui::worker() {
  //serial::println::string("ui::worker()");

  // the first guard will prevent meltdown when the ptr is undef
  if (m_active_card && m_active_card->m_needs_drawing) {
    m_active_card->m_needs_drawing = false;
    m_active_card->draw();
  }

  // do time accounting for card's timeout
  if (m_active_card && m_active_timeout) {
    if (m_active_time_left > 0) { m_active_time_left -= HEARTBEAT; }
    else {
      m_active_card->timeout();
      show(m_active_card->m_timeout_card);
    }
  }
}

void ui::show(const card_index_t& card_index) {
  show(card_index, 0);
}

void ui::show(const card_index_t& card_index, const uint16_t& card_timeout) {
  serial::println::string("ui::show()");

  delete m_active_card; // free the active card's memory
  m_active_index     = card_index; // active card tracker
  m_active_timeout   = card_timeout; // set the card's timeout
  m_active_time_left = card_timeout; // zero the counter

  // simple card factory
  switch (card_index) {
    case CARD_BLANK:
      m_active_card = new CardBlank();
      break;

    case CARD_SPLASH:
      m_active_card = new CardSplash();
      break;

    case CARD_HOME:
      m_active_card = new CardHome();
      break;
  }

  // init() is virtual so, if uneeded, cards may not define it
  m_active_card->init();
}
