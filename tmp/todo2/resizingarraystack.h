/**
 * Arduheater - Telescope heat controller
 * Copyright (C) 2016 João Brázio [joao@brazio.org]
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

#ifndef __RESIZING_ARRAY_STACK_H__
#define __RESIZING_ARRAY_STACK_H__

#include "main.h"

template <typename T>
class ResizingArrayStack
{
private:
  /**
   * The resizing stack pointer
   */
  T* p_items;

  /**
   * The actual number of items on the stack
   */
  uint8_t m_size;

  /**
   * The measurement of the stack from end to end
   */
  uint8_t m_length;

protected:
  /**
   * Protect the copy copnstructor
   */
  ResizingArrayStack(const ResizingArrayStack& rhs) = 0;

  /**
   * Protect the assignment operator
   */
  ResizingArrayStack& operator=(const ResizingArrayStack& rhs) = 0;

  /**
   * Change the capacity of the stack
   * The stack will be grown or shrunk based on the new lenght. Note that if
   * shrinking the stack no "in use" checks will be done thus items can be
   * discarded if the new capacity is less than the number of item existing on
   * the stack.
   *
   * @param newlen new stack length
   */
  void capacity(const uint8_t& newlen)
  {
    m_length = newlen;
    T* p_newitems = new T[newlen];

    for(size_t i = 0; i != m_size; i++)
      p_newitems[i] = p_items[i];

    delete[] p_items;
    p_items = p_newitems;
  }

public:
  /**
   * Constructor
   */
  ResizingArrayStack()
  {
    m_size = 0;
    m_length = 2;
    p_items = new T[2];
  }

  /**
   * Destructor
   */
  ~ResizingArrayStack()
  {
    delete[] p_items;
  }

  /**
   * The actual number of items on the stack
   * @return number of elements on the stack
   */
  const uint8_t& size()
  {
    return m_size;
  }

  /**
   * The measurement of the stack from end to end
   * @return length of the stack
   */
  const uint8_t& length()
  {
    return m_length;
  }

  /**
   * Checks if the stack has no element inside
   * @return true if stack is empty
   */
  bool isEmpty()
  {
    return m_size == 0;
  }

  /**
   * Add a element to the top of the stack
   * @param item the element
   */
  void push(const T& item)
  {
    if (m_size == m_length) capacity(m_length * 2);
    p_items[m_size] = item;
    m_size++;
  }

  /**
   * Get any element from the stack
   * @param  index the element position
   * @return       the requested element
   */
  T get(const uint8_t& index)
  {
    if (isEmpty() || index > m_size) return 0;
    return p_items[index];
  }

  /**
   * Remove a element from the top of the stack
   * @return the element
   */
  T pop()
  {
    if (isEmpty()) return 0;
    m_size--;

    const T item = p_items[m_size];
    p_items[m_size] = 0;

    if (m_size > 0 && m_size == m_length / 4)
      capacity(m_length / 2);

    return item;
  }
};

#endif
