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

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <Arduino.h>
#include "circularqueue.h"

#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 128
#endif

#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 64
#endif

#ifndef BAUDRATE
  #define BAUDRATE 57600
#endif

#ifndef SERIAL_NO_DATA
  #define SERIAL_NO_DATA 0xff
#endif

namespace serial {
  extern CircularQueue<uint8_t, RX_BUFFER_SIZE> rx_buffer;
  extern CircularQueue<uint8_t, TX_BUFFER_SIZE> tx_buffer;

  void init();

  inline uint8_t read() {
    return (rx_buffer.isEmpty())
      ? SERIAL_NO_DATA : rx_buffer.dequeue();
  }

  void write(const uint8_t& s);
};

#endif
