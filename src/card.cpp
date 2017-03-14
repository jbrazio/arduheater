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

void Card::set_slideshow_pages(const uint8_t& pages) {
  m_number_of_pages = pages;  // store the number of pages
  m_active_page = 0;          // set the active page
}

void Card::set_slideshow_period(const uint16_t& ms) {
  m_timer.slideshow.period = utils::mstotick(ms); // convert ms to ticks
  m_register |= bit(CARD_IS_A_SLIDESHOW);         // activate the slideshow flag
}

void Card::next_slideshow_page() {
  if (!bit_is_set(m_register, CARD_IS_A_SLIDESHOW)) { return; }
  m_active_page = (m_active_page +1) % m_number_of_pages; // rollover next page
  m_timer.slideshow.ticks = m_timer.slideshow.period;     // reset the timer
  m_register |= bit(CARD_NEEDS_DRAWING);                  // force screen redraw
}

void Card::set_ui_timeout(const uint16_t& ms) {
  m_timer.ui.period = utils::mstotick(ms);  // store the timeout period
  m_register |= bit(CARD_HAS_UI_TIMEOUT);     // activate the UI flag
}

void Card::keypress(const keycode_t& key, const bool& extended) {
  switch (key) {
    case KEYCODE_LEFT: {
      left(extended);
      break;
    }

    case KEYCODE_RIGHT: {
      right(extended);
      break;
    }

    case KEYCODE_OK: {
      confirm();
      break;
    }

    default: { break; }
  }

  if (bit_is_set(m_register, CARD_HAS_UI_TIMEOUT)) {
    m_timer.ui.ticks = m_timer.ui.period;
  }
}

void Card::needs_drawing(const bool& lhs) {
  if (lhs) { m_register |= bit(CARD_NEEDS_DRAWING); }
}

bool Card::needs_drawing() {
  return bit_is_set(m_register, CARD_NEEDS_DRAWING);
}

void Card::draw() {
  m_register &= ~bit(CARD_NEEDS_DRAWING);
}

void Card::tick() {
  ++m_timer.ticks;

  if (bit_is_set(m_register, CARD_IS_A_SLIDESHOW)) {
    if (m_timer.slideshow.ticks > 0) { --m_timer.slideshow.ticks; }
    else { next_slideshow_page(); }
  }

  if (bit_is_set(m_register, CARD_HAS_UI_TIMEOUT)) {
    if (m_timer.ui.ticks > 0) { --m_timer.ui.ticks; }
    else { reset(); }
  }

  if (bit_is_set(m_register, CARD_NEEDS_DRAWING)) {
    draw();
  }
}
