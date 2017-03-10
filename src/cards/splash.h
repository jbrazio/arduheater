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

#ifndef __CARD_SPLASH_H__
#define __CARD_SPLASH_H__

#include "common.h"

class CardSplash : public Card {
public:
  CardSplash() : Card(1, 0) {;}
  virtual ~CardSplash() {;}

public:
  void draw() {
    Painter::instance()->firstPage();
    do {
      Painter::instance()->setColorIndex(1);
      Painter::instance()->drawBitmapP(0, 0, 16, 64, bitmap_bootlogo);
    } while(Painter::instance()->nextPage());
  }

  void init() { m_timeout_card = CARD_HOME; }
  bool timeout() {
    if (runtime::single::instance().m_ambient.t()
      && runtime::single::instance().m_ambient.rh()
      && runtime::single::instance().m_output[3].t.full()) return true;
    else return false;
  }
};

#endif
