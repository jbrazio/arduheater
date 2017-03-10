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

bool Card::did_page_timeout() {
  if (m_slideshow.period && m_slideshow.timeleft > 0) {
    m_slideshow.timeleft -= HEARTBEAT;
    return false;
  }
  return true;
}

bool Card::did_timeout() {
  if (m_timeout.period && m_timeout.timeleft > 0) {
    m_timeout.timeleft -= HEARTBEAT;
    return false;
  }
  return true;
}

bool Card::has_pages() {
  return (m_pages > 1 && m_slideshow.period > 0);
}

bool Card::has_timeout() {
  return (m_timeout.period > 0);
}

bool Card::needs_drawing(const uint8_t & lhs) {
  if (lhs != 0xff) { m_needs_drawing = lhs; }
  return m_needs_drawing;
}

card_index_t Card::timeout_card() {
  return m_timeout_card;
}

void Card::next_page() {
  m_page_active        = (m_page_active +1) % m_pages;
  m_slideshow.timeleft = m_slideshow.period;
}

void Card::reset_timeout() {
  m_timeout.timeleft = m_timeout.period;
}

void Card::set_timeout(const uint16_t & lhs) {
  m_timeout.period   = lhs; // user defined timeout
  m_timeout.timeleft = lhs; // zero the counter
}
