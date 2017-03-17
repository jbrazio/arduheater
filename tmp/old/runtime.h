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

#include "common.h"

class runtime : public Observer<message_t>
{
public:
  runtime() {;}

public:
  typedef Singleton<runtime> single;

public:
  struct {
    mmsmooth_t<int16_t, 10> dew;
    mmsmooth_t<int16_t, 10> rh;
    mmsmooth_t<int16_t, 10> t;
  } ambient;

  struct {
    PID pid;
    mmsmooth_t<int16_t, 10> t;

    bool running()  { return (pid.mode() == PID::AUTOMATIC); }
    void off()      { pid.mode(PID::MANUAL); pid.output(0); }
    void on()       { pid.mode(PID::AUTOMATIC); }
  } heater[4];

public:
  void update(const message_t&);
};

#endif
