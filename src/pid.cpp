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

// This PID lib is heavly based on the Arduino's PID library by Brett and his
// excelent tutorial published at http://brettbeauregard.com, thus some code
// is (c) 2004 Brett Beauregard <br3ttb@gmail.com>

#include "arduheater.h"

void pid::autotune(const uint8_t& thermistor, const uint8_t& temp, const uint8_t& cycles) {
  bool     heating = true;
  float    Ku, Tu, Wp, Wi, Wd;
  float    max = 0, min = 10000;
  long     th, tl;
  millis_t t0, t1, t2;
  uint8_t  bias, d, n;

  m_output = bias = d = m_max / 2.0;
  t0 = t1 = t2 = 0;
  th = tl = 0;
  n = 0;

  serial::println::PGM(PSTR("PID autotune start"));

  if (ntc.t(thermistor) > temp) {
    serial::println::PGM(PSTR("PID autotune failed"));
    output(0);
    return;
  }

  for (;;) {
    millis_t now = millis();
    float input = ntc.t(thermistor);

    no_less(max, input);
    no_more(min, input);

    if (heating && input > temp) {
      if (now > t2 + 5000UL) {
        heating = false;
        output((bias - d) / 2.0);

        t1 = now;
        th = t1 - t2;
        max = temp;
      }
    }

    if (!heating && input < temp) {
      if (now > t1 + 5000UL) {
        heating = true;
        t2 = now;
        tl = t2 - t1;

        if (n > 0) {
          long maxpower = m_max;
          bias += (d * (th - tl)) / (tl + th);
          bias = constrain(bias, 20, maxpower - 20);
          d = (bias > maxpower / 2.0) ? maxpower - 1 - bias : bias;

          serial::print::pair::int32(PSTR("bias"), bias);
          serial::print::pair::int32(PSTR("d"), d);
          serial::print::pair::float32(PSTR("min"), min, 2);
          serial::print::pair::float32(PSTR("max"), max, 2);

          if (n > 2) {
            Ku = (4.0 * d) / (M_PI * (max - min) * 0.5);
            Tu = ((float) (tl + th) * 0.001);

            serial::print::pair::float32(PSTR("Ku"), Ku, 2);
            serial::print::pair::float32(PSTR("Tu"), Tu, 2);

            Wp = Ku * 0.7;
            Wi = Tu / 2.5;
            Wd = 3 * Tu / 20.0;

            serial::print::pair::float32(PSTR("Wp"), Wp, 2);
            serial::print::pair::float32(PSTR("Wi"), Wi, 2);
            serial::print::pair::float32(PSTR("Wd"), Wd, 2);
          }
        }

        output((bias + d) / 2.0);
        ++n;
        min = temp;
      }
    }

    if (now > t0) {
      serial::print::pair::float32(PSTR("ntc"), ntc.t(thermistor), 2);
      serial::print::pair::float32(PSTR("m_output"), m_output, 2);
      t0 = now + 5000UL;
    }

    if (((now - t1) + (now - t2)) > (10L * 60L * 1000L * 2L)) {
      serial::println::PGM(PSTR("PID autotune 2m timeout"));
      output(0);
      return;
    }

    if (n > cycles) {
      serial::println::PGM(PSTR("PID autotune finished"));
      serial::print::pair::float32(PSTR("Kp"), Wp, 2);
      serial::print::pair::float32(PSTR("Ki"), Wi, 2);
      serial::print::pair::float32(PSTR("Kd"), Wd, 2);
      output(0);
      return;
    }
  }
}

void pid::irq(const bool& reset) {
  if (! m_running) { return; }

  static float s_last_input = 0;            // input value from last cycle
  float error  = m_setpoint - m_input;      // calculate current error
  float dInput = m_input - s_last_input;    // calculate input derivative

  static float s_error = 0;         // integration of error from 0 to present
  s_error += m_Ki * (error * m_dt); // adding the Ki term at this point will
                                    // allow a smooth curve when tuning while
                                    // running.

  if (reset) { s_error = m_output; dInput = 0; }  // reboot the algorithm

  if (s_error > m_max) { s_error = m_max; }       // cap the I term between
  else if (s_error < m_min) { s_error = m_min; }  // min and max values

  // evaluate the PID algorithm
  float u = (m_Kp * error) + s_error - (m_Kd * (dInput / m_dt));

  if (u > m_max) { u = m_max; }       // cap the output between
  else if (u < m_min) { u = m_min; }  // min and max values

  m_output = u;
  s_last_input = m_input;
}

void pid::limit(const float& min, const float& max) {
  if (min > max) { return; }
  m_min = min;
  m_max = max;
}

void pid::output(const float& lhs) {
  m_output = lhs;
  m_output = constrain(m_output, m_min, m_max);
}

void pid::tune(const float& Np, const float& Ni, const float& Nd) {
  // To use the PID into an inverted state i.e. output decreases with the
  // increase of error just provide negative paramaters.
  Kp(Np); Ki(Ni); Kd(Nd);
}
