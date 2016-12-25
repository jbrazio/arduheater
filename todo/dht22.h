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

#ifndef __DHT22_H__
#define __DHT22_H__

#include "state.h"
#include "hardware.h"
#include "sensordata.h"

class DHT22: public Hardware<float> {
private:
  uint8_t pin;
  volatile float data[2];
  const uint32_t refreshTime = 1999L;

protected:
  float getDewPoint()
  {
    const float celsius = this->data[TEMPERATURE];
    const float humidity = this->data[HUMIDITY];

    #ifdef NOAA_DEW_FUNCTION
      // dewPoint function NOAA
      // reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
      // reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm

      // (1) Saturation Vapor Pressure = ESGG(T)
      const float RATIO = 373.15 / (273.15 + celsius);
      float RHS = -7.90298 * (RATIO - 1);
      RHS += 5.02808 * log10(RATIO);
      RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
      RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
      RHS += log10(1013.246);

      // factor -3 is to adjust units - Vapor Pressure SVP * humidity
      const float VP = pow(10, RHS - 3) * humidity;

      // (2) DEWPOINT = F(Vapor Pressure)
      const float T = log(VP/0.61078);   // temp var
      return (241.88 * T) / (17.558 - T);
    #else
      // delta max = 0.6544 wrt dewPoint()
      // 6.9 x faster than dewPoint function NOAA
      // reference: http://en.wikipedia.org/wiki/Dew_point
      const float temp = (17.271f * celsius) / (237.7f + celsius) + log(humidity * 0.01);
      return (237.7f * temp) / (17.271f - temp);
    #endif
  }

public:
  DHT22(const uint8_t &pin): Hardware() {
    this->pin = pin;
  }

  virtual bool setup() {
    this->sensorState = STATE_READY;
    return true;
  }

  bool refresh() {
    if (this->state() != STATE_READY) return false;

    this->sensorState = STATE_BUSY;
    this->lastReadTime = millis();

    // Send the start signal
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, LOW);

    delay(1);

    // Switch into date receive mode
    pinMode(this->pin, INPUT);
    digitalWrite(this->pin, HIGH);

    if (! pulseIn(this->pin, LOW, 115)) {
      this->sensorState = STATE_ERROR;
      return false;
    }

    uint16_t data = 0;
    uint16_t rawHumidity = 0;
    uint16_t rawTemperature = 0;

    for(uint8_t i = 0; i < 40; i++) {
      data <<= 1;
      if (pulseIn(this->pin, HIGH, 200) > 30) data |= 1;

      switch (i) {
        case 15:
          rawHumidity = data;
          break;

        case 31:
          rawTemperature = data;
          data = 0;
      }
    }

    // Verify checksum
    if ((byte)(((byte)rawHumidity) + (rawHumidity >> 8)
      + ((byte)rawTemperature) + (rawTemperature >> 8)) != data) {
      this->sensorState = STATE_ERROR;
      return false;
    }

    // Store readings
    this->data[1] = rawHumidity * 0.1;

    if ( rawTemperature & 0x8000 ) {
      rawTemperature = -(int16_t)(rawTemperature & 0x7FFF);
    }

    this->data[0] = ((int16_t)rawTemperature) * 0.1;

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

      case HUMIDITY:
        return this->data[1];

      case DEW:
        return this->getDewPoint();

      default:
        return NAN;
    }
  }
};

#endif
