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

  // if our status isn't RUNNING then no need to evaluate the
  // output channels.
  if (! (sys.state & RUNNING)) return;

  uint8_t totalactive = 0;
  uint16_t totalpower = 0;

  // total error count accumulates errors from all the sensors
  static uint8_t total_err_count = 0;

  // invalid data count accumulates per channel the number of invalid
  // readings since the last successful one, when it reaches the threshold
  // triggers increases the above accumulator by one.
  static uint8_t invalid_data_count[NUM_OUTPUTS] = { 0, 0, 0, 0 };

  const float t = amb.t() + amb.config.t_offset;
  const float h = roundf(amb.rh() + amb.config.rh_offset);  // DHT22 has 2 to 5% error
  const float d = utils::dew(t, h) + amb.config.dew_offset;

  for (size_t channel = 0; channel < NUM_OUTPUTS; channel++) {
    if (! ntc.is_fresh(channel)) { continue; }

    const float     in = ntc.t(channel);
    const float    raw = ntc.raw(channel);
    const bool   ready = ntc.is_ready(channel);
    const bool enabled = (sys.output & (OUTPUT0_ENABLED << channel));

    // thermal protection
    // checks if the raw data is out of bounds
    if (raw != THERMISTOR_ERR_TEMP && (raw >= THERMISTOR_MIN_VAL || raw <= THERMISTOR_MAX_VAL)) {
      out[channel].alg.stop();
      digitalWrite(output_pin(channel), LOW);
      sys.output &= ~(OUTPUT0_ENABLED << channel);

      serial::print::PGM(PSTR("warn: out"));
      serial::print::uint8(channel);
      serial::println::PGM(PSTR(" output disabled, out of bounds"));

      serial::print::PGM(PSTR("warn: t:"));
      serial::print::uint8(ntc.t(channel));
      serial::print::PGM(PSTR(", raw:"));
      serial::println::float32(raw, 2);

      ++total_err_count;
      continue;
    }

    // thermal protection
    // check if sensor is in error but the output is enabled
    if (! ready && enabled) {
      if (invalid_data_count[channel] >= THERMISTOR_ERR_THRESHOLD) {
        out[channel].alg.stop();
        digitalWrite(output_pin(channel), LOW);
        sys.output &= ~(OUTPUT0_ENABLED << channel);

        serial::print::PGM(PSTR("warn: out"));
        serial::print::uint8(channel);
        serial::println::PGM(PSTR(" output disabled, invalid data threshold"));

        serial::print::PGM(PSTR("warn: t:"));
        serial::print::uint8(ntc.t(channel));
        serial::print::PGM(PSTR(", raw:"));
        serial::println::float32(raw, 2);

        invalid_data_count[channel] = 0;
        ++total_err_count;
        continue;
      }

      else {
        serial::print::PGM(PSTR("warn: out"));
        serial::print::uint8(channel);
        serial::print::PGM(PSTR(" invalid sensor data received ("));
        serial::print::uint8(invalid_data_count[channel] +1);
        serial::print::PGM(PSTR("/"));
        serial::print::uint8(THERMISTOR_ERR_THRESHOLD);
        serial::println::PGM(PSTR(")"));

        ++invalid_data_count[channel];
        continue;
      }
    }

    if (ready) {
      // clears the invalid accumulator
      invalid_data_count[channel] = 0;

      // update the setpoint
      const float s = roundf(d + out[channel].config.offset);
      out[channel].alg.setpoint(s);

      if (enabled) {
        // sync PID and output status
        if (! out[channel].alg.active()) {
          out[channel].alg.start();
        }

        // evaluate the pid
        else {
          out[channel].alg.input(in);
          out[channel].alg.irq();

          ++totalactive;
          totalpower += out[channel].alg.output();
        }
      }

      else {
        // shtudown the output
        out[channel].alg.stop();
        digitalWrite(output_pin(channel), LOW);
        sys.output &= ~(OUTPUT0_ENABLED << channel);
      }
    }
  }

  if (total_err_count >= 10) {
    serial::println::PGM(PSTR("err: thermal protection, too many errors"));
    disable_all_outputs();
    halt();
  }

  for (size_t channel = 0; channel < NUM_OUTPUTS; channel++) {
    const bool   ready = ntc.is_ready(channel);
    const bool enabled = (sys.output & (OUTPUT0_ENABLED << channel));

    if (ready && enabled && out[channel].alg.active()) {
      if (totalpower > 510) {
        out[channel].alg.output((out[channel].alg.output() / totalpower) * 510);

        /*
        serial::print::PGM(PSTR("warn: out"));
        serial::print::uint8(channel);
        serial::print::PGM(PSTR(" output cap to "));
        serial::println::uint8(out[channel].alg.output());
        */
      }

      analogWrite(output_pin(channel), out[channel].alg.output());
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
