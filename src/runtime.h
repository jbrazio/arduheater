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

#ifndef __RUNTIME_H__
#define __RUNTIME_H__

#include <Arduino.h>
#include "observer.h"
#include "singleton.h"
#include "smooth_mm.h"
#include "struct.h"

class runtime : public Observer<message_t> {
public:
  runtime()
    : m_lcd_needs_refresh(true)
  {;}

public:
  typedef Singleton<runtime> single;

public:
  bool m_lcd_needs_refresh;

  struct ambient_data_t {
    mmsmooth_t<int16_t, 10> dew;
    mmsmooth_t<int16_t, 10> rh;
    mmsmooth_t<int16_t, 10> t;
  } m_ambient;

public:
  void update(const message_t&);
};

#endif
