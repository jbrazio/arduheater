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

#ifndef ARDUINO
  // Timer0 interrupt handler
  ISR(TIMER0_OVF_vect)
  {
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    unsigned long m = timer0_millis;
    unsigned char f = timer0_fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
      f -= FRACT_MAX;
      m += 1;
    }

    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
  }
#endif

// Timer1 interrupt handler
ISR(TIMER1_COMPA_vect)
{
  ntc.irq();
  amb.irq();

  // the following section will calculate the total required
  // power by the active outputs
  uint16_t totalpower = 0;
  for (size_t i = 0; i < NUM_OUTPUTS; i++) {
    if (ntc_ready(i)) {
      // thermal protection
      if (ntc.t(i) >= HEATER_MAX_TEMP) {
        serial::print::PGM(PSTR("warn: out"));
        serial::print::uint8(i);
        serial::println::PGM(PSTR(" max temp thermal protection"));
        disable_all_outputs();
        halt();
      }

      // if the output is active evaluate the pid
      else if (out[i].alg.active()) {
        out[i].alg.input(ntc.t(i));
        out[i].alg.irq();
        totalpower += out[i].alg.output();
      }
    }

    else {
      // thermal protection
      if (out[i].alg.active()) {
        serial::print::PGM(PSTR("warn: out"));
        serial::print::uint8(i);
        serial::println::PGM(PSTR(" thermal protection"));
        disable_all_outputs();
        halt();
      }
    }
  }

  // the following section will cal
  for (size_t i = 0; i < NUM_OUTPUTS; i++) {
    if (ntc_ready(i) && out[i].alg.active()) {
      if (totalpower > 255) {
        out[i].alg.output((out[i].alg.output() / totalpower) * 255);
        serial::print::PGM(PSTR("warn: out"));
        serial::print::uint8(i);
        serial::print::PGM(PSTR(" output capped to "));
        serial::println::uint8(out[i].alg.output());
      }
      analogWrite(output_pin(i), out[i].alg.output());
    }
  }
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
  uint8_t c = UDR0; // read a byte
  // check for parity errors and filter for ASCII chars
  if (bit_is_clear(UCSR0A, UPE0) && c <= 0x7f) {
    //TODO: trigger an alarm on buffer overflow
    serial::buffer.rx.enqueue(c);
  } /* else { discard byte } */
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
