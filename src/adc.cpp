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

adc_t adc::runtime;

void adc::selchan(const uint8_t& channel) {
  sys.state &= ~(STATE_ADC_COMPLETE);

  runtime.channel = channel;  // store the active channel
  runtime.value = -1;         // reset last reading
  ADMUX = (channel & 0x07);   // select adc channel
}

void adc::update() {
  ADCSRA |= bit(ADSC) | bit(ADIE);
}
