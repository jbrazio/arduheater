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

void pid::irq(const bool& reset) {
  if (m_mode != pid::AUTOMATIC) { return; }

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

void pid::mode(const pid::mode_t& lhs) {
  // force algorithm reboot if going into automatic mode from manual mode
  if (lhs == pid::AUTOMATIC && m_mode == pid::MANUAL) { irq(true); }
  m_mode = lhs;
}

void pid::tune(const float& Kp, const float& Ki, const float& Kd) {
  // To use the PID into an inverted state i.e. output decreases with the
  // increase of error just provide negative paramaters.
  m_Kp = Kp; m_Ki = Ki; m_Kd = Kd;
}

void pid::output(const float& lhs) {
  if (m_mode != pid::MANUAL) { return; }
  m_output = lhs;
}

float pid::output() {
  return m_output;
}

pid::mode_t pid::mode() {
  return m_mode;
}

void pid::input(const float& lhs) {
  m_input = lhs;
}

void pid::sampletime(const float& lhs) {
  m_dt = lhs / 1000L;
}

void pid::setpoint(const float& lhs) {
  m_setpoint = lhs;
}

void pid::autotune(const uint8_t& thermistor, const uint8_t& temp, const uint8_t& cycles) {
  uint8_t n = 0;

  float   input = 0.0;
  bool    heating = true;

  float   Ku = 0.0;
  float   Tu = 0.0;
  float   Wp = 0.0;
  float   Wi = 0.0;
  float   Wd = 0.0;

  float   max = 0.0;
  float   min = 10000;

  long    bias, d;

  millis_t t0 = millis();
  millis_t t1 = t0;  // when input > temp
  millis_t t2 = t0;  // when input < temp
  millis_t th;  //
  millis_t tl;  //

  m_output = bias = d = m_max / 2.0;

  serial::println::PGM(PSTR("PID autotune start"));

  for (;;) {
    millis_t ms = millis();

    input = ntc.t(thermistor);

    no_less(max, input);
    no_more(min, input);

    if (heating && input > temp) {
      if (ms > t2 + 5000UL) {
        heating = false;
        m_output = (bias - d) / 2.0;

        t1 = ms;
        th = t1 - t2;
        max = temp;
      }
    }

    if (!heating && input < temp) {
      if (ms > t1 + 5000UL) {
        heating = true;
        t2 = ms;
        tl = t2 - t2;

        if (n > 0) {
          long maxpower = m_max;
          bias += (d * (th -tl)) / (tl + th);
          bias = constrain(bias, 20, maxpower - 20);
          d = (bias > maxpower /2) ? maxpower - 1 - bias : bias;

          serial::print::pair::uint32(PSTR("bias"), bias);
          serial::print::pair::uint32(PSTR("d"), d);
          serial::print::pair::uint32(PSTR("min"), min);
          serial::print::pair::uint32(PSTR("max"), max);

          if (n > 2) {
            Ku = (4.0 * d) / (M_PI * (max - min) * 0.5);
            Tu = ((float) (tl - th) * 0.001);

            serial::print::pair::uint32(PSTR("Ku"), Ku);
            serial::print::pair::uint32(PSTR("Tu"), Tu);

            Wp = 0.6 * Ku;
            Wi = 2 * Wp / Tu;
            Wd = Wp * Tu * 0.125;

            serial::print::pair::uint32(PSTR("Wp"), Wp);
            serial::print::pair::uint32(PSTR("Wi"), Wi);
            serial::print::pair::uint32(PSTR("Wd"), Wd);
          }
        }

        m_output = (bias + d) / 2.0;
        ++n;
        min = temp;
      }
    }

    if (ms > t0 + 5000UL) {
      serial::print::pair::float32(PSTR("ntc"), ntc.t(thermistor), 2);
      //serial::print::pair::float32(PSTR("m_output"), m_output, 2);

      /*serial::print::pair::float32(PSTR("input"), input, 2);
      serial::print::pair::uint32(PSTR("t0"), t0);
      serial::print::pair::uint32(PSTR("t1"), t1);
      serial::print::pair::uint32(PSTR("t2"), t2);
      serial::print::pair::uint32(PSTR("ms"), ms);*/
      t0 = ms;
    }

    if (((ms - t1) + (ms - t2)) > (10L * 60L * 1000L * 2L)) {
      serial::println::PGM(PSTR("PID autotune timeout"));
      return;
    }

    if (n > cycles) {
      serial::println::PGM(PSTR("PID autotune finished"));
      serial::print::pair::uint32(PSTR("Kp"), Wp);
      serial::print::pair::uint32(PSTR("Ki"), Wi);
      serial::print::pair::uint32(PSTR("Kd"), Wd);
      return;
    }
  }
}
