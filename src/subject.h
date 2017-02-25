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

#ifndef __SUBJECT_H__
#define __SUBJECT_H__

#include "common.h"

/**
 * Abstract subject/publisher class
 */
template <typename T> class Subject
{
private:
  Observer<T>* p_observer;

protected:
  Subject() {
    p_observer = nullptr;
  }

  virtual ~Subject() {;}

public:
  /**
   * Register an observer for our subject
   */
  void attach(Observer<T>* obs)
  {
    if (p_observer == nullptr)
      p_observer = obs;
  }

  /**
   * Unregister an existing observer
   */
  void dettach()
  {
    if (p_observer != nullptr)
      p_observer = nullptr;
  }

protected:
  /**
   * Notify the observer
   */
  void notify(const T& message)
  {
    if (p_observer != nullptr)
      p_observer->update(message);
  }
};

#endif
