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

// System global control structures
dht22       amb;
out_t       out[NUM_OUTPUTS];
thermistor  ntc;
volatile system_t sys;

int main(void)
{
  // --------------------------------------------------------------------------
  // Enable global interrupts -------------------------------------------------
  // --------------------------------------------------------------------------
  sei();


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
  // Sensor init --------------------------------------------------------------
  // --------------------------------------------------------------------------
  ntc.init();
  amb.init();


  // --------------------------------------------------------------------------
  // Timer0 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // set waveform generation mode to Fast PWM
  TCCR0A |= bit(WGM01) | bit(WGM00);

  // set clock select to 64 (from prescaler)
  TCCR0B |= bit(CS01) | bit(CS00);

  // set overflow interrupt enable
  TIMSK0 |= bit(TOIE0);


  // --------------------------------------------------------------------------
  // Timer1 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // set output compare register A to 20Hz
  OCR1A = 0xC35;

  // set waveform generation mode to CTC
  TCCR1B |= bit(WGM12);

  // set clock select to 256 (from prescaler)
  TCCR1B |= bit(CS12);

  // set output compare A match interrupt enable
  TIMSK1 |= bit(OCIE1A);


  // --------------------------------------------------------------------------
  // Timer2 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // set waveform generation mode to PWM Phase Correct
  TCCR2A |= bit(WGM20);

  // set clock select to 64 (from prescaler)
  TCCR2B |= bit(CS22);


  // --------------------------------------------------------------------------
  // ADC init routine ---------------------------------------------------------
  // --------------------------------------------------------------------------
  // set ADC prescaler select to 128
  ADCSRA |= bit(ADPS2) | bit(ADPS1) | bit(ADPS0);

  // set ADC Enable
  ADCSRA |= bit(ADEN);

  // set ADC multiplexer selection to internal 1.1V
  ADMUX  |= bit(REFS1) | bit(REFS0);

  #if (NUM_OUTPUTS > 3)
    // set digital input disable register to A4-A5
    DIDR0 |= bit(ADC5D) | bit(ADC4D);
  #elif (NUM_OUTPUTS > 2)
    // set digital input disable register to A3-A5
    DIDR0 |= bit(ADC5D) | bit(ADC4D) | bit(ADC3D);
  #elif (NUM_OUTPUTS > 1)
    // set digital input disable register to A2-A5
    DIDR0 |= bit(ADC5D) | bit(ADC4D) | bit(ADC3D) | bit(ADC2D);
  #elif (NUM_OUTPUTS > 0)
    // set digital input disable register to A1-A5
    DIDR0 |= bit(ADC5D) | bit(ADC4D) | bit(ADC3D) | bit(ADC2D) | bit(ADC1D);
  #endif


  // --------------------------------------------------------------------------
  // Load settings ------------------------------------------------------------
  // --------------------------------------------------------------------------
  eeprom::load();


  // --------------------------------------------------------------------------
  // Startup check ------------------------------------------------------------
  // --------------------------------------------------------------------------
  while (!any_ntc_ready()) {
    millis_t now = utils::millis();
    static millis_t next = now + 5000L;
    if (now > next) {
      serial::println::PGM(PSTR("warn: no outputs available"));
      break;
    } else { utils::delay(1); }
  }

  while (!(sys.status & STATUS_AMBIENT_READY)) {
    millis_t now = utils::millis();
    static millis_t next = now + 10000L;
    if (now > next) {
      serial::println::PGM(PSTR("err: ambient sensor error"));
      halt();
    } else { utils::delay(1); }
  }

  serial::banner();


  // --------------------------------------------------------------------------
  // Output init --------------------------------------------------------------
  // --------------------------------------------------------------------------
  DDRB |= 0x20; // Enable D13 as output
  for (size_t i = 0; i < NUM_OUTPUTS; i++ ) {
    pinMode(output_pin(i), OUTPUT); // Enable PWM outputs
    if (out[i].config.autostart) {
      serial::print::PGM(PSTR("warn: auto started out"));
      serial::println::uint8(i);
      cmd::enableheater(i);
    }
  }


  // --------------------------------------------------------------------------
  // Wathcdog -----------------------------------------------------------------
  // --------------------------------------------------------------------------
  //wdt_enable(WDTO_4S);


  // --------------------------------------------------------------------------
  // Loop routine -------------------------------------------------------------
  // --------------------------------------------------------------------------
  for(;;) {
    wdt_reset();
    serial::process();

    millis_t now = millis();
    static millis_t next = 0;

    if (now > next) {
      next = now + 500L;
      serial::print::float32(ntc.t(0), 2);
      serial::print::chr::space();
      serial::print::float32(out[0].alg.setpoint(), 2);
      serial::print::chr::space();
      serial::print::float32(out[0].alg.output(), 2);
      serial::print::chr::eol();
    }
  }

  // We should not reach this
  return 0;
}
