/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2018 João Brázio [joao@brazio.org]
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
  ENABLE_SCOPE_DEBUG(13); // Timer 1 CTC
  ENABLE_SCOPE_DEBUG(12); // INT0 ISR
  ENABLE_SCOPE_DEBUG(8);  // ADC


  // --------------------------------------------------------------------------
  // Disable global interrupts ------------------------------------------------
  // --------------------------------------------------------------------------
  cli();


  // --------------------------------------------------------------------------
  // Serial port init routine -------------------------------------------------
  // --------------------------------------------------------------------------
  Serial::setup();


  // --------------------------------------------------------------------------
  // Timer0 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  // set waveform generation mode to Fast PWM
  TCCR0A |= bit(WGM01) | bit(WGM00);

  // set clock select to 64 (from prescaler)
  TCCR0B |= bit(CS01) | bit(CS00);

  // enable timer0 overflow interrupt
  TIMSK0 |= bit(TOIE0);


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
  // Startup check ------------------------------------------------------------
  // --------------------------------------------------------------------------
  while (! Environment::is_ready()) {
    // Delay 16 000 cycle: 1ms at 16.0 MHz
    asm volatile (
      "    ldi  r18, 21"  "\n"
      "    ldi  r19, 199" "\n"
      "1:  dec  r19"      "\n"
      "    brne 1b"       "\n"
      "    dec  r18"      "\n"
      "    brne 1b"       "\n"
    );
  }
  LogLn::PGM(PSTR("INFO: Ambient sensor ready."));
  Log::eol();


  // --------------------------------------------------------------------------
  // Banner -------------------------------------------------------------------
  // --------------------------------------------------------------------------
  LogLn::PGM(PSTR("Arduheater " ARDUHEATER_VERSION " ['$' for help]"));
  LogLn::PGM(PSTR("Visit " ARDUHEATER_URL " for updates."));
  Log::eol();


  // --------------------------------------------------------------------------
  // Load settings ------------------------------------------------------------
  // --------------------------------------------------------------------------
  //eeprom::load();
  //

  // --------------------------------------------------------------------------
  // Output init --------------------------------------------------------------
  // --------------------------------------------------------------------------
  for (size_t i = 0; i < 4; i++ ) {
    IO::set_as_output(get_heater_pin(i));

    /*if (out[i].config.autostart) {
      Log::PGM(PSTR("warn: auto started out"));
      LogLn::number(i);
      cmd::enableheater(i);
    }*/
  }


  // --------------------------------------------------------------------------
  // Watchdog -----------------------------------------------------------------
  // --------------------------------------------------------------------------
  //wdt_enable(WDTO_4S);


  // --------------------------------------------------------------------------
  // Loop routine -------------------------------------------------------------
  // --------------------------------------------------------------------------
  for(;;) {
    //wdt_reset();
    Serial::process(&protocol::process);

    #ifdef DEBUG
      uint32_t now = micros();
      static uint32_t before = now;

      if(now - before > 1e6) {
        before = now;

          Log::PGM(PSTR("#"));
          Log::number(Environment::get_temperature());
          Log::PGM(PSTR(","));
          Log::number(Environment::get_humidity());
          Log::PGM(PSTR(","));
          Log::number(Environment::get_dew_point());

        for(size_t i = 0; i < 4; i++) {
          // Ignore output if it is disconnected
          //if (raw == 1023) continue;
          if (! Output::channel(i).is_connected()) continue;

          const uint16_t raw = Output::channel(i).sensor.raw();
          const float t = Output::channel(i).sensor.temp();

          // Format
          // <Channel>,<ADC raw value>,<Sensor temp>,<Target temp>

          Log::PGM(PSTR(":"));
          //Log::number(i +1);
          //Log::PGM(PSTR(","));
          Log::number(raw);
          Log::PGM(PSTR(","));
          Log::number(t);

          Heater::runtime_t dump = Output::channel(i).heater.dump_runtime();

          Log::PGM(PSTR(","));
          Log::number(dump.target);
          Log::PGM(PSTR(","));
          Log::number((float)dump.target/10);
          Log::PGM(PSTR(","));
          Log::number(dump.Perr);
          Log::PGM(PSTR(","));
          Log::number(dump.Ierr);
          Log::PGM(PSTR(","));
          Log::number(dump.Derr);
          Log::PGM(PSTR(","));
          Log::number(dump.P);
          Log::PGM(PSTR(","));
          Log::number(dump.I);
          Log::PGM(PSTR(","));
          Log::number(dump.D);
          Log::PGM(PSTR(","));
          Log::number(dump.u);
          Log::PGM(PSTR(","));
          Log::number(Output::channel(i).heater.get_value());
        }

        Log::eol();
      }
    #endif
  }

  // We should not reach this
  return 0;
}
