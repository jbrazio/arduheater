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

#include <Arduino.h>
#include "serial.h"

CircularQueue<uint8_t, RX_BUFFER_SIZE> serial::rx_buffer;
CircularQueue<uint8_t, TX_BUFFER_SIZE> serial::tx_buffer;

void serial::init() {
  // Set baud rate
  #if BAUDRATE < 57600
    uint16_t UBRR0_value = ((F_CPU / (8L * BAUDRATE)) - 1)/2 ;
    UCSR0A &= ~(1 << U2X0); // baud doubler off  - Only needed on Uno XXX
  #else
    uint16_t UBRR0_value = ((F_CPU / (4L * BAUDRATE)) - 1)/2;
    UCSR0A |= (1 << U2X0);  // baud doubler on for high baud rates, i.e. 115200
  #endif
  UBRR0H = UBRR0_value >> 8;
  UBRR0L = UBRR0_value;

  // enable rx and tx
  UCSR0B |= 1<<RXEN0;
  UCSR0B |= 1<<TXEN0;

  // enable interrupt on complete reception of a byte
  UCSR0B |= 1<<RXCIE0;

  // defaults to 8-bit, no parity, 1 stop bit
}

void serial::write(const uint8_t& data) {
/*
  // Wait until there is space in the buffer
  while (tx_buffer.isFull()) {
    __asm__("nop\n\t");
  }
*/

  tx_buffer.enqueue(data);

  // Enable Data Register Empty Interrupt
  // to make sure tx-streaming is running
  UCSR0B |=  (1 << UDRIE0);
}

ISR(USART_RX_vect) {
  uint8_t data = UDR0;

  switch (data) {
    default:
      // Write data to buffer unless it is full
      //TODO: else alarm on overflow?
      serial::rx_buffer.enqueue(data);
  }
}

// Data Register Empty Interrupt handler
ISR(USART_UDRE_vect) {
  // Send a byte from the buffer
  UDR0 = serial::tx_buffer.dequeue();

  // Turn off Data Register Empty Interrupt
  //  to stop tx-streaming if this concludes the transfer
  if (serial::tx_buffer.isEmpty()) { UCSR0B &= ~(1 << UDRIE0); }
}
