/**
 * Arduheater - Telescope heat controller
 * Copyright (C) 2017 João Brázio [joao@brazio.org]
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

#ifndef __RUNTIME_H__
#define __RUNTIME_H__

class runtime
{
public:
  typedef Singleton<runtime> single;

  struct runtime_lcd_t {
    struct page_t {
      volatile bool v_outdated;
      volatile uint8_t v_timeout;
      volatile menu_page_t v_next;
      volatile menu_page_t v_current;
    } page;
  } lcd;

  void init() {;}
};

#endif
