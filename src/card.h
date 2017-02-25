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

#ifndef __CARD_H__
#define __CARD_H__

#include "common.h"

class Card {
protected:
  Card()
    : m_needs_drawing(true)
    , m_page_period(10000L)
    , m_page_active(0)
    , m_pages(0)
  {;}

public:
  virtual ~Card() {;}

protected:
  bool          m_needs_drawing;
  card_index_t  m_timeout_card;
  hb_timer_t    m_slideshow;
  hb_timer_t    m_timeout;
  uint16_t      m_page_period;
  uint8_t       m_page_active;
  uint8_t       m_pages;

public:
  virtual void draw()    {;}
  virtual void init()    {;}
  virtual void left()    {;}
  virtual void press()   {;}
  virtual void right()   {;}
  virtual void timeout() {;}

public:
  bool did_page_timeout();
  bool did_timeout();
  bool has_pages();
  bool has_timeout();
  bool needs_drawing(const uint8_t & lhs = 0xff);
  card_index_t timeout_card();
  void next_page();
  void reset_timeout();
  void set_timeout(const uint16_t & lhs = 0);
};

#endif
