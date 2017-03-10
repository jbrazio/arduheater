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

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "common.h"

/**
 * Singleton class
 * This class follows the Meyer's Singleton Pattern using variadic template
 */
template<class T> class Singleton
{
protected:
  Singleton() {;}
  virtual ~Singleton() {;}

/*
private:
  Singleton(Singleton const&);
  Singleton& operator=(Singleton const&);
*/
public:
  Singleton(Singleton const&) = delete;
  Singleton& operator=(Singleton const&) = delete;

public:
  /**
   * This method generates a single instance and returns a reference to it.
   *
   * Static properties are only instantiated on their first use, thus declaring
   * s_instance as static will provide lazy initialization.
   *
   * Do not use this class as a parent of another class, instead declare a
   * public property such as:
   *  typedef Singleton<A_CLASS_NAME> single
   *
   * Then the invocation and lazy initilization of the singleton can be done by
   * using CLASS::single::instance().
   *
   * @return An instance reference of the T class.
   */
  template<typename... A> inline static T& instance(A... args) {
    static T s_instance(args...); // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    return s_instance;
  }
};

template<class C, typename... A> C& singleton(A... args) {
  static C instance(args...); // Guaranteed to be destroyed.
                              // Instantiated on first use.
  return instance;
}

#endif
