/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2019 João Brázio [joao@brazio.org]
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

int main(void)
{
  // --------------------------------------------------------------------------
  // Miscellaneous ------------------------------------------------------------
  // --------------------------------------------------------------------------
  //ENABLE_SCOPE_DEBUG(12); // PB4
  //ENABLE_SCOPE_DEBUG(13); // PB5


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
  TCCR0A = 0; TCCR0B = 0;

  // set waveform generation mode to PWM, Phase Correct, 8-bit (D5, D6)
  TCCR0A |= bit(WGM00);

  // set clock select to clk/1024 (~30Hz)
  TCCR0B |= bit(CS02) | bit(CS00);


  // --------------------------------------------------------------------------
  // Timer1 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  TCCR1B = 0; TIMSK1 = 0;

  // set waveform generation mode to CTC, top OCR1A (D9, D10)
  TCCR1B |= bit(WGM12);

  // set clock select to clk/64
  TCCR1B |= bit(CS11) | bit(CS10);

  // output Compare A Match Interrupt Enable
  TIMSK1 |= bit(OCIE1B) | bit(OCIE1A);

  // sets the Output Compare Register values
  OCR1A = 0xFF; // (977-980Hz ~1KHz)
  OCR1B = 0x80; // (200Hz)


  // --------------------------------------------------------------------------
  // Timer2 ISR init routine --------------------------------------------------
  // --------------------------------------------------------------------------
  TCCR2A = 0; TCCR2B = 0;

  // set waveform generation mode to PWM, Phase Correct, 8-bit (D3, D11)
  TCCR2A |= bit(WGM20);

  // set clock select to clk/1024 (~30Hz)
  TCCR2B |= bit(CS22) | bit(CS21) | bit(CS20);


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
  LogLn::PGM(PSTR("I Checking for ambient sensor."));
  while (! ambient.is_ready()) {
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
  LogLn::PGM(PSTR("I Ambient sensor ready."));
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
      Log::PGM(PSTR("W Auto started heater #"));
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
  }

  // We should not reach this
  return 0;
}
