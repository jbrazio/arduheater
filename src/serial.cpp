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

#include "common.h"

CircularQueue<uint8_t, RX_BUFFER_SIZE> serial::rx_buffer;
CircularQueue<uint8_t, TX_BUFFER_SIZE> serial::tx_buffer;

void serial::init() {
  // set baud rate
  #if BAUDRATE < 57600
    uint16_t UBRR0_value = ((F_CPU / (8L * BAUDRATE)) - 1)/2 ;
    UCSR0A &= ~(1 << U2X0); // baud doubler off  - Only needed on Uno
  #else
    uint16_t UBRR0_value = ((F_CPU / (4L * BAUDRATE)) - 1)/2;
    UCSR0A |= (1 << U2X0);  // baud doubler on for high baud rates, i.e. 115200
  #endif
  UBRR0H = UBRR0_value >> 8;
  UBRR0L = UBRR0_value;

  // enable rx and tx
  UCSR0B |= (1<<RXEN0);
  UCSR0B |= (1<<TXEN0);

  // enable interrupt on complete reception of a byte
  UCSR0B |= (1<<RXCIE0);

  // defaults to 8-bit, no parity, 1 stop bit
}

inline uint8_t serial::read() {
  return (serial::rx_buffer.empty())
    ? SERIAL_NO_DATA : rx_buffer.dequeue();
}

void serial::write(const uint8_t& data) {
  // wait until there is space in the buffer
  while (!serial::tx_buffer.enqueue(data)) {
    // at this point interrupts are disabled so we need to
    // manually poll the data register empty flag
    if (bit_is_clear(SREG, SREG_I)) {
      if (bit_is_set(UCSR0A, UDRE0)) { serial::irq::tx(); }
    }
  }

  // Enable Data Register Empty Interrupt
  // to make sure tx-streaming is running
  UCSR0B |= (1 << UDRIE0);
}

void serial::irq::rx() {
  // check for parity errors
  if (bit_is_clear(UCSR0A, UPE0)) {
    uint8_t data = UDR0; // read a byte

    switch (data) {
      default: {
        // write data to buffer unless it is full
        //TODO: trigger an alarm or overflow
        serial::rx_buffer.enqueue(data);
        break;
      }
    }
  } else { /* discard */ UDR0; }
}

void serial::irq::tx() {
  // send a byte from the buffer
  UDR0 = serial::tx_buffer.dequeue();

  UCSR0A |= (1 << TXC0);

  // turn off Data Register Empty Interrupt
  // to stop tx-streaming if this concludes the transfer
  if (serial::tx_buffer.empty()) { UCSR0B &= ~(1 << UDRIE0); }
}

// RX Interrupt handler
ISR(USART_RX_vect) { serial::irq::rx(); }

// Data Register Empty Interrupt handler
ISR(USART_UDRE_vect) { serial::irq::tx(); }
