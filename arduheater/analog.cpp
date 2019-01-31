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

#include "analog.h"

/**
 * @brief Static class member initialization
 */
Analog::callback_t Analog::s_callback = NULL;
Analog::buffer_t Analog::s_buffer = { 0, 0, {} };

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
ISR(ADC_vect)
{
  SCOPE_DEBUG_OUTPUT(4);

  // store the raw value from the ADC
  Analog::add_to_buffer((uint16_t)ADCW);

  // after reaching the sample count filter the results
  if(Analog::is_buffer_full()) {
    CRITICAL_SECTION_START
      // we could shave a few uS from the ISR by using a inline static
      // function instead of the lambda, but who cares ?
      qsort(Analog::get_buffer_ptr(), Analog::buffer_size(), sizeof(uint16_t),
        [](const void* a, const void* b) {
          return ( *(int*)a - *(int*)b );
        }
      );

      // uses a basic 95th percentile filter with an additional discarding
      // of the lower 5% results.
      uint32_t sum = 0;
      const uint8_t start = Analog::buffer_size() * 0.05f; // lower 5%
      const uint8_t end   = Analog::buffer_size() - start; // sort of 95th percentile
      for(uint8_t i = start; i < end; i++) { sum += Analog::get_from_buffer(i); }
      const uint16_t value = (uint16_t) (sum / (end - start));

      // local cache the volatile system var
      const uint8_t channel = Analog::get_channel();
    CRITICAL_SECTION_END

    // notify any observer of the change
    Analog::callback(channel, value);

    //SCOPE_DEBUG_OUTPUT(5);
  }

  else { ADCSRA |= bit(ADSC) | bit(ADIE); }

  SCOPE_DEBUG_OUTPUT(4);
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void Analog::read(const uint8_t& channel, const callback_t callback)
{
  if(channel > 3) { return; }

  CRITICAL_SECTION_START
    clear_buffer();
    set_channel(channel);
    set_callback(callback);

    // select the internal 1.1V aref and the target analog channel
    ADMUX = bit (REFS1) | bit (REFS0) | (channel & 0x07);

    // start the async analog read
    ADCSRA |= bit(ADSC) | bit(ADIE);
  CRITICAL_SECTION_END
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 */
void Analog::setup()
{
  CRITICAL_SECTION_START
    // clear adc prescaler bits
    ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));

    #ifdef ADC_PRESCALER16
      // set ADC prescaler select to 16 (upto 76,923 conversions/sec)
      ADCSRA |= bit (ADPS2);
    #else
      // set ADC prescaler select to 128 (upto 9,615 conversions/sec)
      ADCSRA |= bit(ADPS2) | bit(ADPS1) | bit(ADPS0);
    #endif

    // set ADC Enable
    ADCSRA |= bit(ADEN);

    // set ADC multiplexer selection to internal 1.1V
    ADMUX  |= bit(REFS1) | bit(REFS0);

    // set digital input disable register to A4-A5
    DIDR0 |= bit(ADC5D) | bit(ADC4D);
  CRITICAL_SECTION_END

  // Delay 80 000 cycles
  // 5ms at 16.0 MHz
  asm volatile (
      "    ldi  r18, 104" "\n"
      "    ldi  r19, 229" "\n"
      "1:  dec  r19"  "\n"
      "    brne 1b" "\n"
      "    dec  r18"  "\n"
      "    brne 1b" "\n"
  );
}
