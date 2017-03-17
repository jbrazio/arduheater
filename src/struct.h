/**
 * Arduheater - Heat controller for astronomy usage
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

#ifndef __STRUCT_H__
#define __STRUCT_H__

struct system_t {
  uint8_t state;
  uint8_t status;
};

struct serial_buffer_t {
  CircularQueue<uint8_t, SERIAL_RX_BUFFER_SIZE> rx;
  CircularQueue<uint8_t, SERIAL_TX_BUFFER_SIZE> tx;
};

struct adc_t {
  uint8_t channel;
  int16_t value;
};

#endif
