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

void DHT22::update() {
  m_state = SENSOR_BUSY;

  // send the start signal and switch into receive mode
  pinMode(m_pin, OUTPUT);
  digitalWrite(m_pin, LOW);

  // trying to keep out the need of delay, millis & co.
  for (volatile uint16_t i = 0; i < 16000; i++) {
    __asm__ __volatile__ ("nop");
  }

  digitalWrite(m_pin, HIGH);
  pinMode(m_pin, INPUT);

  // return timeout if no data is received
  // TODO: stop using pulsein
  if (! pulseIn(m_pin, LOW, 115)) {
    m_state = SENSOR_TIMEOUT;
    return;
  }

  uint16_t data = 0;
  uint16_t rawHumidity = 0;
  uint16_t rawTemperature = 0;

  // process the incoming data
  // TODO: stop using pulsein
  for(uint8_t i = 0; i < 40; i++) {
    data <<= 1;
    if (pulseIn(m_pin, HIGH, 200) > 30) data |= 1;

    switch (i) {
      case 15:
        rawHumidity = data;
        break;

      case 31:
        rawTemperature = data;
        data = 0;
    }
  }

  // checksum validation
  if ((byte)(((byte)rawHumidity) + (rawHumidity >> 8)
    + ((byte)rawTemperature) + (rawTemperature >> 8)) != data) {
    m_state = SENSOR_ERROR;
    return;
  }

  // cache the received humidity data
  m_cache[1] = rawHumidity * 0.1;

  if (rawTemperature & 0x8000) {
    rawTemperature = -(int16_t)(rawTemperature & 0x7FFF);
  }

  // cache the received temperature data
  m_cache[0] = ((int16_t)rawTemperature) * 0.1;

  reset();

  message_t payload;
  payload.category  = MSG_CAT_WEATHER;
  payload.data[0].f = m_cache[0];
  payload.data[1].f = m_cache[1];
  notify(payload);
}
