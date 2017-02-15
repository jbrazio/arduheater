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

#ifndef __CARD_HOME_H__
#define __CARD_HOME_H__

#include <Arduino.h>
#include "card.h"
#include "config.h"
#include "painter.h"
#include "print.h"
#include "runtime.h"

#include "widgets/title.h"
#include "widgets/middle.h"

class CardHome : public Card {
public:
  CardHome() {;}
  virtual ~CardHome() {;}

public:
  void draw() {
    char buffer[4];
    uint8_t x, y;

    Painter::instance()->firstPage();
    do {
      widget::title::draw(string_lcd_ambient,  1);
      widget::title::draw(string_lcd_humidity, 2);

      widget::middle::draw(1);


    } while(Painter::instance()->nextPage());
  }
};

#endif
