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

#include "isr.h"

/**
 * @brief Local variable initialization
 */
volatile uint32_t timer1_compa_count = 0;

/**
 * @brief Timer1 Compare Match A handler
 * @details
 *
 */
ISR(TIMER1_COMPA_vect)
{
  ++timer1_compa_count; // 4us increment
}

/**
 * @brief Timer1 Compare Match B handler
 * @details An 100Hz ISR gives 100 blocks per second, each block has a specific
 * execution window of 10ms, a full update uses 4 blocks thus the overall expected
 * refresh rate is 25 times per second.
 *
 */
ISR(TIMER1_COMPB_vect)
{
  // To use timer1 OCF1A and OCF1B we have to some calculations in order to only
  // trigger OCF1B at the 200Hz frequency: Timer1 runs in CTC mode top 0xFF
  // (OCR1A) at 1/64 clock speed, this means each increment on OCR1A is equal to
  // 4 us, an "overflow" i.e. OCR1A == 0xFF is equal to 1024 us thus OCF1A is
  // running at ~1KHz frequency. We want OCF1B to run at ~100Hz, take the frequency
  // of OCF1A (1024Hz) and divide it by 100Hz to get the magic number bellow (10).
  static uint8_t timer1_compb_vect_counter = 255;
  if(++timer1_compb_vect_counter < 128) { return; }
  timer1_compb_vect_counter = 0;

  // This is the main state machine which drives all the recurring tasks on the
  // program such as trigger thermistor readings, refresh heater output, evaluate
  // PID etc.
  static uint8_t state_machine_step = 255;
  ++state_machine_step %= 5;

//SCOPE_DEBUG_OUTPUT(5);
  switch (state_machine_step)
  {
    // Thermistor update trigger
    case 0:
    case 1:
    case 2:
    case 3:
      Analog::read(get_sensor_pin(state_machine_step), output_callback);
      break;

    // Ambient sensor update trigger
    case 4: // read ambient
      ambient.read(environment_calback);
      break;

    default:
      ;
  }

  // The process to update each output power is independent from any of the previous
  // callbacks allowing us to respond to disable() without additional processing.
  for(size_t i = 0; i < asizeof(output); i++) {
    const uint8_t channel = output[i].channel();
    const uint8_t power   = (output[i].is_enabled()) ? output[i].output_value() : 0;
    IO::write(get_heater_pin(channel), (output[i].is_ready()) ? power : 0);
  }
//SCOPE_DEBUG_OUTPUT(5);
}

/**
 * @brief External Interrupt Request Vector
 * @details PORT D, PD2, INT0
 */
ISR (INT0_vect)
{
  //SCOPE_DEBUG_OUTPUT(5);
  ambient.isr();
  //SCOPE_DEBUG_OUTPUT(5);
}
