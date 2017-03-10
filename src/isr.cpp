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

/**
 * Analog to Digital Converter interrupt handler
 */
ISR(ADC_vect) { thermistor::single::instance().isr(ADC); }

/**
 * Pin Change interrupt handler for PORTB aka 2
 */
ISR(PCINT0_vect) { keypad::single::instance().isr(2); }

/**
 * Pin Change interrupt handler for PORTC aka 3
 */
//ISR(PCINT1_vect) { keypad::single::instance().isr(3); }

/**
 * Pin Change interrupt handler for PORTD aka 4
 */
//ISR(PCINT2_vect) { keypad::single::instance().isr(4); }

/**
 * Timer1 interrupt handler
 */
ISR(TIMER1_COMPA_vect) {
  static volatile uint8_t busy = false; // Binary semaphore
  if (busy) { return; }                 // Prevent ISR (re)triggering
  PORTB |= 0x20;                        // Set D13 high
  busy = true;                          // Acquire the lock

  keypad::single::instance().irq();
  DHT22::single::instance().irq();
  thermistor::single::instance().irq();
  ui::single::instance().irq();

  for (size_t i = 0; i < 1; i++) {
    runtime::output_t* p = &runtime::single::instance().m_output[i];
    p->pid.input(p->t());
    p->pid.irq();
    analogWrite(HEATER_A_PIN, p->pid.output());
  }

  busy = false;   // Release the lock
  PORTB &= ~0x20; // Set D13 low
}
