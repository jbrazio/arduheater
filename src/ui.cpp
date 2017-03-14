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
  if (m_active_card && m_active_card->needs_drawing()) {
    DEBUGPRN(5, "ui::draw(): needs_drawing()");
    m_active_card->draw();
  }
}

void ui::irq() {
  // this guard will prevent meltdown when the ptr is undef
  if (m_active_card) { m_active_card->tick(); }
}

void ui::show(const card_index_t& card_index) {
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
  //m_active_card->set_timeout(card_timeout); // define the card's timeout
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
      m_active_card->keypress(static_cast<keycode_t>(message.data[0].dw), message.data[1].bol);
      break;
    }

    default: { break; }
  }
}
