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

#ifndef __CIRCULARQUEUE_H__
#define __CIRCULARQUEUE_H__

#include <Arduino.h>

/**
 * @brief   Circular Queue class
 * @details Implementation of the classic ring buffer data structure
 */
template<typename T, uint8_t N> class CircularQueue {
private:
  /**
   * @brief   Buffer structure
   * @details This structure consolidates all the overhead required to handle
   *          a circular queue such as the pointers and the buffer vector.
   */
  volatile struct {
    T queue[N];
    uint8_t count;
    uint8_t head;
    uint8_t size;
    uint8_t tail;
  } m_buffer;

public:
  /**
   * @brief   Class constructor
   * @details This class requires two template parameters, T defines the type
   *          of item this queue will handle and N defines the maximum number of
   *          items that can be stored on the queue.
   */
   CircularQueue<T, N>() {
     m_buffer.count = 0;
     m_buffer.head  = 0;
     m_buffer.size  = N;
     m_buffer.tail  = 0;
   }

   /**
    * @brief Gets the number of items on the queue
    * @details Returns the current number of items stored on the queue.
    * @return number of items in the queue
    */
   inline uint8_t count() { return m_buffer.count; }

  /**
   * @brief   Adds an item to the queue
   * @details Adds an item to the queue on the location pointed by the buffer_t
   *          tail variable. Returns false if no queue space is available.
   * @param   item Item to be added to the queue
   * @return  true if the operation was successful
   */
   bool enqueue(T const &item) {
     if (isFull()) { return false; }
     ++m_buffer.count;
     m_buffer.queue[m_buffer.tail] = item;
     if (++m_buffer.tail == m_buffer.size) { m_buffer.tail = 0; }
     return true;
   }

  /**
   * @brief   Checks if the queue has no items
   * @details Returns true if there are no items on the queue, false otherwise.
   * @return  true if queue is empty
   */
  inline bool isEmpty() { return (m_buffer.count == 0); }

  /**
   * @brief   Checks if the queue is full
   * @details Returns true if the queue is full, false otherwise.
   * @return  true if queue is full
   */
  inline bool isFull() { return (m_buffer.count == m_buffer.size); }

  /**
   * @brief   Gets the next item from the queue without removing it
   * @details Returns the next item in the queue without removing it
   *          or updating the pointers.
   * @return  first item in the queue
   */
  inline T peek() { return m_buffer.queue[m_buffer.head]; }

  /**
   * @brief   Gets the queue size
   * @details Returns the maximum number of items a queue can have.
   * @return  the queue size
   */
  inline uint8_t size() { return m_buffer.size; }

  /**
   * @brief   Removes and returns a item from the queue
   * @details Removes the oldest item on the queue, pointed to by the
   *          buffer_t head field. The item is returned to the caller.
   * @return  type T item
   */
   T dequeue() {
     if (isEmpty()) { return T(); }
     --m_buffer.count;
     T item = m_buffer.queue[m_buffer.head];
     if (++m_buffer.head == m_buffer.size) { m_buffer.head = 0; }
     return item;
   }
};

#endif
