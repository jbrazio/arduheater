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

#include "arduheater.h"

// Timer1 interrupt handler
ISR(TIMER1_COMPA_vect)
{
  ntc.irq();
  amb.irq();

  for (size_t i = 0; i < NUM_OUTPUTS; i++) { out[i].alg.irq(); }

  analogWrite(HEATER_0_PIN, out[0].alg.output());
}

// Analog to Digital Converter interrupt handler
ISR(ADC_vect)
{
  adc::runtime.value = ADCW;
  sys.state |= ADC_READING_DONE;
}

// Serial RX interrupt handler
ISR(USART_RX_vect)
{
  // check for parity errors
  if (bit_is_clear(UCSR0A, UPE0)) {
    uint8_t data = UDR0; // read a byte

    switch (data) {
      default: {
        // write data to buffer unless it is full
        //TODO: trigger an alarm or overflow
        serial::buffer.rx.enqueue(data);
        break;
      }
    }
  } else { /* discard */ UDR0; }
}

// Serial TX interrupt handler
ISR(USART_UDRE_vect)
{
  // send a byte from the buffer
  UDR0 = serial::buffer.tx.dequeue();
  UCSR0A |= bit(TXC0);

  // turn off Data Register Empty Interrupt
  // to stop tx-streaming if this concludes the transfer
  if (serial::buffer.tx.empty()) { UCSR0B &= ~bit(UDRIE0); }
}
