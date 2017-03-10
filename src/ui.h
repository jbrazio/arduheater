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

#ifndef __UI_H__
#define __UI_H__

#include "common.h"

class ui : public Observer<message_t>
{
public:
  typedef Singleton<ui> single;

public:
  ui();

private:
  Card* m_active_card;
  card_index_t m_active_index;

public:
  inline void show(const card_index_t& card_index) { show(card_index, 0); }

public:
  void draw();
  void irq();
  void show(const card_index_t&, const uint16_t&);
  void update(const message_t&);
};

#endif
