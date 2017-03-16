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

#define CARD_IS_A_SLIDESHOW 0
#define CARD_HAS_UI_TIMEOUT 1
#define CARD_NEEDS_TIMEOUT  2
#define CARD_NEEDS_DRAWING  7

class Card {
protected:
  /*
   * The m_register property is a 8bit "register" with the following flags:
   *
   * bit 0 - CARD_IS_A_SLIDESHOW
   * This flag bit is set when the card has more than one page and needs to
   * cycle among them.
   *
   * bit 1 - CARD_HAS_UI_TIMEOUT
   * This flag bit is set when the card can react to user inputs such as the
   * press of a key and has a timeout after the last keypress.
   *
   * bit 2 - CARD_NEEDS_TIMEOUT
   * This flag bit is set when a pending UI timeout is active.
   *
   * bit 7 - CARD_NEEDS_DRAWING
   * This flag bit is set when the card needs to be redrawn due to a update.
   *
   */
  uint8_t m_register;

protected:
  Card() {
    m_timer.ticks = 0;
  }

public:
  virtual ~Card() {;}

protected:
  struct {
    tick_timer_t slideshow;
    tick_timer_t ui;
    uint8_t ticks;
  } m_timer;

protected:
  uint8_t m_number_of_pages;
  uint8_t m_active_page;

public:
  void set_slideshow_pages(const uint8_t& pages);
  void set_slideshow_period(const uint16_t& ms);
  virtual void next_slideshow_page();

public:
  void keypress(const keycode_t& key, const bool& extended);
  void set_ui_timeout(const uint16_t& ms);
  virtual void confirm() {;}
  virtual void left(__attribute__((unused)) const bool& extended) {;}
  virtual void timeout() {;}
  virtual void right(__attribute__((unused)) const bool& extended) {;}

public:
  bool needs_drawing();
  void needs_drawing(const bool& lhs);
  virtual void draw() {;}
  virtual void init() {;}
  virtual void tick();
};

#endif
