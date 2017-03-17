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

// Declare system global variable structure
system_t sys;

int main(void)
{
  DDRB |= 0x20; // Enable D13 as output
  memset(&sys, 0, sizeof(system_t));  // Clear all system variables

  // --------------------------------------------------------------------------
  // Serial port init routine -------------------------------------------------
  // --------------------------------------------------------------------------
  #if SERIAL_BAUDRATE < 57600
    uint16_t UBRR0_value = ((F_CPU / (8L * SERIAL_BAUDRATE)) - 1) /2;
    UCSR0A &= ~bit(U2X0); // baud doubler off (required by UNO)
  #else
    uint16_t UBRR0_value = ((F_CPU / (4L * SERIAL_BAUDRATE)) - 1) /2;
    UCSR0A |= bit(U2X0);  // baud doubler on for high baud rates
  #endif

  // set baudrate
  UBRR0H = UBRR0_value >> 8;
  UBRR0L = UBRR0_value;

  // enable rx and tx
  UCSR0B |= bit(RXEN0);
  UCSR0B |= bit(TXEN0);

  // enable interrupt on complete reception of a byte
  UCSR0B |= bit(RXCIE0);

  // set serial status as ready
  sys.status |= STATUS_SERIAL_READY;


  // --------------------------------------------------------------------------
  // Timer1 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  TCCR1A  = TCCR1B = TCNT1 = 0;         // clears timer1 registers
  OCR1A   = 0xC35;                      // sets the frequency to 20Hz
  TCCR1B |= bit(WGM12) | bit(CS12);     // enable CTC mode with a 256 prescaler
  TIMSK1 |= bit(OCIE1A);                // enable the compare interrupt


  // --------------------------------------------------------------------------
  // ADC init routine ---------------------------------------------------------
  // --------------------------------------------------------------------------
  ADCSRA  = bit(ADEN);                            // activate the ADC
  ADCSRA |= bit(ADPS0) | bit(ADPS1) | bit(ADPS2); // with prescaler of 128
  ADMUX   = bit(REFS0) | bit(REFS1);              // select aref 1.1V

  #if NUM_OUTPUT_CHANNELS == 1
    // disable digital buffer from A1 to A3
    DIDR0 |= ADC1D | ADC2D | ADC3D;
  #elif NUM_OUTPUT_CHANNELS == 2
    // disable digital buffer from A2 to A3
    DIDR0 |= ADC2D | ADC3D;
  #elif NUM_OUTPUT_CHANNELS == 3
    // disable digital buffer on A3
    DIDR0 |= ADC3D;
  #endif

  // disable digital buffer from A4 to A5
  DIDR0 |= ADC4D | ADC5D;

  // clears the ADC runtime structure
  memset(&adc::runtime, 0, sizeof(adc_t));


  // --------------------------------------------------------------------------
  // Enable interrupts --------------------------------------------------------
  // --------------------------------------------------------------------------
  sei();

  // set ISR status as ready
  sys.status |= STATUS_ISR_READY;


  // --------------------------------------------------------------------------
  // Miscellaneous ------------------------------------------------------------
  // --------------------------------------------------------------------------
  serial::banner();


  // --------------------------------------------------------------------------
  // Loop routine -------------------------------------------------------------
  // --------------------------------------------------------------------------
  for(;;) {

    serial::process();

  }

  // We should not reach this
  return 0;
}
