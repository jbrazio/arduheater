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

void ui::draw() {
  if (m_active_card->needs_drawing()) {
    DEBUGPRN(5, "ui::draw(): needs_drawing()");
    m_active_card->needs_drawing(false);
    m_active_card->draw();
  }
}

void ui::irq() {
  // this guard will prevent meltdown when the ptr is undef
  if (m_active_card) {
    // do time accounting for card's timeout
    if (m_active_card->has_timeout() && m_active_card->did_timeout()) {
      DEBUGPRN(5, "ui::irq(): timeout_card()");
      if (m_active_card->timeout()) show(m_active_card->timeout_card());
      else m_active_card->reset_timeout();
    }

    // do time accounting for card's slideshow
    if (m_active_card->has_pages() && m_active_card->did_page_timeout()) {
      DEBUGPRN(5, "ui::irq(): next_page()");
      m_active_card->needs_drawing(true);
      m_active_card->next_page();
    }

    /*
    static uint16_t foo = 0;
    if (foo >= 250) {
      m_active_card->needs_drawing(true);
      foo = 0;
    } else foo += HEARTBEAT;
    */
  }
}

void ui::show(const card_index_t& card_index, const uint16_t& card_timeout) {
  delete m_active_card; // free the active card's memory

  // simple card factory
  switch (card_index) {
    case CARD_BLANK: {
      DEBUGPRN(6, "ui::show(): new CardBlank()");
      m_active_card = new CardBlank();
      break;
    }

    case CARD_SPLASH: {
      DEBUGPRN(6, "ui::show(): new CardSplash()");
      m_active_card = new CardSplash();
      break;
    }

    case CARD_HOME: {
      DEBUGPRN(6, "ui::show(): new CardHome()");
      m_active_card = new CardHome();
      break;
    }
  }

  m_active_card->init();                    // let the card bootstrap itself
  m_active_index = card_index;              // bookeeping
  m_active_card->set_timeout(card_timeout); // define the card's timeout
}

void ui::update(const message_t& message) {
  switch (message.category) {
    case MSG_UPDATE_LCD: {
      DEBUGPRN(4, "ui::update(): MSG_UPDATE_LCD");
      m_active_card->needs_drawing(true);
      break;
    }

    case MSG_CAT_KEYPAD: {
      DEBUGPRN(3, "ui::update(): MSG_CAT_KEYPAD");

      keycode_t  button = static_cast<keycode_t>(message.data[0].dw);
      keypress_t state  = static_cast<keypress_t>(message.data[1].dw);

      if (state == KEYPRESS_SHORT) {
        if (button == KEYCODE_RIGHT) { m_active_card->right(); }
        else if (button == KEYCODE_LEFT) { m_active_card->left(); }
      }

      // reset the card's timeout if needed
      if (m_active_card->has_timeout()) { m_active_card->reset_timeout(); }
      m_active_card->needs_drawing(true);
      break;
    }

    default: { break; }
  }
}
