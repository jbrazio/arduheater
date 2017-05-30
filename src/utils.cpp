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

uint8_t utils::msectotick(const uint16_t& ms)
{
  // WARNING: max 12750ms as input
  return (ms / HEARTBEAT);
}

float utils::steinhart(const uint16_t& raw)
{
  float ret = constrain(raw, 1, 1022);
  ret  = THERMISTOR_SERIESRESISTOR / (1023.0 / ret - 1);  // convert raw to ohms
  ret  = ret / THERMISTOR_NOMINAL_VAL;                    // (R/Ro)
  ret  = log(ret);                                        // ln(R/Ro)
  ret /= THERMISTOR_BCOEFFICIENT;                         // 1/B * ln(R/Ro)
  ret += 1.0 / (THERMISTOR_NOMINAL_TEMP + 273.15);        // + (1/To)
  ret  = 1.0 / ret;                                       // invert
  ret -= 273.15;                                          // convert to K to C
  return ret;
}

float utils::dew(const float& t, const float& rh)
{
  // Uncomment the following line to enable the NOAA dew function
  // otherwise a simpler calculating algorithm will be used.
  //#define NOAA_DEW_FUNCTION

  #ifdef NOAA_DEW_FUNCTION
    // dewPoint function NOAA
    // reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
    // reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm

    // (1) Saturation Vapor Pressure = ESGG(T)
    const float RATIO = 373.15 / (273.15 + t);
    float RHS = -7.90298 * (RATIO - 1);
    RHS += 5.02808 * log10(RATIO);
    RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
    RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
    RHS += log10(1013.246);

    // factor -3 is to adjust units - Vapor Pressure SVP * rh
    const float VP = pow(10, RHS - 3) * rh;

    // (2) DEWPOINT = F(Vapor Pressure)
    const float T = log(VP/0.61078);   // temp var
    return (241.88 * T) / (17.558 - T);
  #else
    // delta max = 0.6544 wrt dewPoint()
    // 6.9 x faster than dewPoint function NOAA
    // reference: http://en.wikipedia.org/wiki/Dew_point
    const float temp = (17.271f * t) / (237.7f + t) + log(rh * 0.01);
    return (237.7f * temp) / (17.271f - temp);
  #endif
}

millis_t utils::millis()
{
  #ifdef ARDUINO
    return ::millis();
  #else
    unsigned long m;
    uint8_t oldSREG = SREG;

    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer0_millis)
    cli();

    m = timer0_millis;
    SREG = oldSREG;
    return m;
  #endif
}

void utils::delay(uint32_t ms)
{
  uint32_t start = micros();

  while (ms > 0) {
    wdt_reset();
    while (ms > 0 && (micros() - start) >= 1000) {
      ms--;
      start += 1000;
    }
  }
}
