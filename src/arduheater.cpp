/**
 * Arduheater - An intelligent dew buster for astronomy
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

// Global control structures
/*dht22 amb;
thermistor ntc;
out_t out[NUM_OUTPUTS];
volatile system_t sys;
*/

int main(void)
{
  // --------------------------------------------------------------------------
  // Miscellaneous ------------------------------------------------------------
  // --------------------------------------------------------------------------
  //DDRB |= 0x30; // Enable D12 and D13 as output
  DDRB |= 0x30; // D13, D12, D11
  DDRD |= 0x68; // D6, D5, D3


  // --------------------------------------------------------------------------
  // Disable global interrupts ------------------------------------------------
  // --------------------------------------------------------------------------
  cli();


  // --------------------------------------------------------------------------
  // Serial port init routine -------------------------------------------------
  // --------------------------------------------------------------------------
  Serial::setup();


  // --------------------------------------------------------------------------
  // Sensor init --------------------------------------------------------------
  // --------------------------------------------------------------------------
  //ntc.init();
  //amb.init();


  // --------------------------------------------------------------------------
  // Timer0 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // set waveform generation mode to Fast PWM
  TCCR0A |= bit(WGM01) | bit(WGM00);

  // set clock select to 64 (from prescaler)
  TCCR0B |= bit(CS01) | bit(CS00);

  // set overflow interrupt enable
  //TIMSK0 |= bit(TOIE0);


  // --------------------------------------------------------------------------
  // Timer1 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // set output compare register A to 200Hz
  OCR1A = 0x138;

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
  Analog::setup();


  // --------------------------------------------------------------------------
  // Enable global interrupts -------------------------------------------------
  // --------------------------------------------------------------------------
  sei();


  // --------------------------------------------------------------------------
  // Banner -------------------------------------------------------------------
  // --------------------------------------------------------------------------
  Consoleln::PGM(PSTR("Arduheater " ARDUHEATER_VERSION " ['$' for help]"));
  Consoleln::PGM(PSTR("Visit " ARDUHEATER_URL " for updates."));
  Console::eol();


  // --------------------------------------------------------------------------
  // Load settings ------------------------------------------------------------
  // --------------------------------------------------------------------------
  //eeprom::load();


  // --------------------------------------------------------------------------
  // Startup check ------------------------------------------------------------
  // --------------------------------------------------------------------------
  /*
  while (!any_ntc_ready()) {
    millis_t now = utils::millis();
    static millis_t next = now + 5000L;
    if (now > next) {
      Consoleln::PGM(PSTR("warn: no outputs available"));
      break;
    } else { utils::delay(1); }
  }

  while (!(sys.sensor & AMBIENT_SENSOR_READY)) {
    millis_t now = utils::millis();
    static millis_t next = now + 10000L;
    if (now > next) {
      Consoleln::PGM(PSTR("err: ambient sensor error"));
      halt();
    } else { utils::delay(1); }
  }

  // set the status as running
  sys.state |= RUNNING;
  */


  // --------------------------------------------------------------------------
  // Output init --------------------------------------------------------------
  // --------------------------------------------------------------------------

  /*
  for (size_t i = 0; i < NUM_OUTPUTS; i++ ) {
    pinMode(output_pin(i), OUTPUT); // Enable PWM outputs
    if (out[i].config.autostart) {
      Console::PGM(PSTR("warn: auto started out"));
      Consoleln::number(i);
      cmd::enableheater(i);
    }
  }
  */


  // --------------------------------------------------------------------------
  // Watchdog -----------------------------------------------------------------
  // --------------------------------------------------------------------------
  //wdt_enable(WDTO_4S);


  // --------------------------------------------------------------------------
  // Loop routine -------------------------------------------------------------
  // --------------------------------------------------------------------------
  for(;;) {
    //wdt_reset();
    //PORTB ^= 1 << 5;
    Serial::process(&protocol::process);
    //PORTB ^= 1 << 5;

    static uint16_t counter = 0;

    if(++counter == 0) {
      for(size_t i = 0; i < 4; i++) {
        const uint16_t raw = Output::channel(i).sensor.raw();
        const float t = Output::channel(i).sensor.temp();

        if(raw == 1023) continue;

        Console::PGM(PSTR("chan: "));
        Console::number(i +1);
        Console::PGM(PSTR(", val: "));
        Console::number(raw);
        Console::PGM(PSTR(" ("));
        Console::number(t);
        Console::PGM(PSTR("C)"));
        //Console::eol();

        Heater::runtime_t dump = Output::channel(i).heater.dump_runtime();

        Console::PGM(PSTR(", target: "));
        Console::number(dump.target);

        Console::PGM(PSTR(", Perr: "));
        Console::number(dump.Perr);

        Console::PGM(PSTR(", Ierr: "));
        Console::number(dump.Ierr);

        Console::PGM(PSTR(", Derr: "));
        Console::number(dump.Derr);

        Console::PGM(PSTR(", P: "));
        Console::number(dump.P);

        Console::PGM(PSTR(", I: "));
        Console::number(dump.I);

        Console::PGM(PSTR(", D: "));
        Console::number(dump.D);

        Console::PGM(PSTR(", u: "));
        Console::number(dump.u);

        Console::PGM(PSTR(", pwm: "));
        Console::number(constrain(dump.u, 0, 255));

        Console::eol();
      }
      Console::eol();
    }
  }

  // We should not reach this
  return 0;
}
