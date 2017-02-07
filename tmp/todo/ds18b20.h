/**
 * Arduheater - Telescope heat controller
 * Copyright (C) 2016 João Brázio [joao@brazio.org]
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

#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <OneWire.h>

#include "state.h"
#include "hardware.h"

class DS18B20: public Hardware<float> {
private:
  uint8_t pin;
  OneWire wire;
  byte address[8];
  volatile float data[1];
  const uint8_t precision = 0x1F;

protected:
  bool ready() {
    for (uint8_t i = 0; i < 8; i++)
      if (this->address[i] != 0x00) return true;
    return false;
  }

public:
  DS18B20(const uint8_t &pin): Hardware(), wire(pin) {
    this->pin = pin;
  }

  bool setup() {
    byte addr[8];
    char buffer[64];

    sprintf_P(buffer,
      PSTR("DS18B20: Scanning on PIN %d"), pin);
    Serial.println(buffer);

    while(this->wire.search(addr)) {
      if (OneWire::crc8(addr, 7) == addr[7]) {
        Serial.print(F("DS18B20: Found: "));

        for (uint8_t i = 0; i < 8; i++) {
          sprintf_P(buffer, PSTR("0x%X "), addr[i]);
          Serial.print(buffer);
        }

        memcpy(this->address, addr, sizeof(addr));
        Serial.println();
        break;
      } else {
        Serial.println(F("DS18B20: invalid CRC"));
      }
    }

    Serial.println(F("DS18B20: Scan complete"));
    this->wire.reset_search();

    if (! this->ready()) {
      Serial.println(F("DS18B20: No devices found"));
      this->sensorState = STATE_UNKNOWN;
      return false;
    }

    if (this->address[0] == 0x10) {
      this->wire.reset();
      this->wire.select(this->address);
      this->wire.write(0x4E); // Write
      this->wire.write(this->precision);
      this->wire.reset();
      this->wire.select(this->address);
      this->wire.write(0x48); // Copy

      delay(20);
      this->wire.reset();
    }

    this->sensorState = STATE_READY;
    this->lastReadTime = 0;
    return true;
  }

  bool refresh() {
    if (this->state() != STATE_READY) return false;

    this->sensorState = STATE_BUSY;
    this->lastReadTime = millis();

    this->wire.reset();
    this->wire.select(this->address);
    this->wire.write(0x44, 1); // take new reading

    delay(750);
    this->wire.reset();
    this->wire.select(this->address);
    this->wire.write(0xBE); // Read

    byte data[12];
    for (uint8_t i = 0; i < 9; i++) {
      data[i] = this->wire.read();
    }

    int16_t rawTemperature =
      (((int16_t) data[1]) << 11) |
      (((int16_t) data[0]) << 3);

    if (this->address[0] == 0x10) {
      rawTemperature =
        ((rawTemperature & 0xfff0) << 3) - 16 +
        (((data[7] - data[6]) << 7) / data[7]);
    }

    this->data[0] =
      (rawTemperature <= -7040) ? -127 : (float) rawTemperature * 0.0078125;

    this->sensorState = STATE_SLEEP;
    return true;
  }

  float get() {
    return this->get(TEMPERATURE);
  }

  float get(const uint8_t &what) {
    switch(what) {
      case TEMPERATURE:
        return this->data[0];

      default:
        return NAN;
    }
  }
};

#endif
