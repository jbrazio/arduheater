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

void pid::autotune() {
  // WARNING ! This method is not working properly,
  // do not trust the K values it returns.
  //
  // This PID autotune is heavly based on the Arduino's PID library by Brett and
  // his excelent tutorial published at http://brettbeauregard.com, thus some code
  // is (c) 2004 Brett Beauregard <br3ttb@gmail.com>

  serial::println::PGM(PSTR("PID autotune start"));

  m_tunning = true;

  millis_t runtime = 0;
  millis_t status  = 0;
  millis_t peak1   = 0;
  millis_t peak2   = 0;

  while(m_input == 0) { utils::delay(1000); }

  const float oStep = 20;
  const float noiseBand = 0.5;
  const float outputStart = m_output;
  const float setpoint = m_input + 5.0F;
  const int16_t nLookBack = 200; // assuming 10sec lookback
  const int16_t sampleTime = 50; // assuming 10sec lookback
  const millis_t timeout = millis() + 1000L * 60L * 15L;

  bool justchanged = false;
  float absMax = m_input;
  float absMin = m_input;
  float lastInputs[255];
  float peaks[10];
  int16_t peakCount = 0;
  int16_t peakType = 0;

  serial::print::PGM(PSTR("PID autotune min: "));
  serial::print::float32(setpoint - noiseBand, 2);
  serial::print::PGM(PSTR("C, med: "));
  serial::print::float32(setpoint, 2);
  serial::print::PGM(PSTR("C, max: "));
  serial::print::float32(setpoint + noiseBand, 2);
  serial::println::PGM(PSTR("C"));

  for (;;) {
    const millis_t now = utils::millis();

    if (now > status) {
      status = now + 15000L;
      cmd::status();
    }

    if (now > timeout) {
      serial::println::PGM(PSTR("PID autotune timeout"));
      output(outputStart);
      m_tunning = false;
      return;
    }

    if (now > runtime) {
      runtime = now + sampleTime;
      const float refVal = m_input;

      if (refVal > absMax) { absMax = refVal; }
      if (refVal < absMin) { absMin = refVal; }

      // oscillate the output base on the input's relation to the setpoint
      if (refVal > setpoint + noiseBand) {
        serial::print::PGM(PSTR("-"));
        output(outputStart - oStep); }

      else if (refVal < setpoint - noiseBand) {
        serial::print::PGM(PSTR("+"));
        output(outputStart + oStep); }

      else {
        serial::print::PGM(PSTR("."));
      }

      bool isMax = true;
      bool isMin = true;

      //id peaks
      for (int i = nLookBack - 1; i >= 0; i--) {
        float val = lastInputs[i];
        if (isMax) { isMax = (refVal > val); }
        if (isMin) { isMin = (refVal < val); }
        lastInputs[i+1] = lastInputs[i];
      }

      lastInputs[0] = refVal;

      if (nLookBack > 9) {
        if (isMax) {
          if (peakType == 0) { peakType = 1; }
          else if (peakType == -1) {
            peakType = 1;
            justchanged = true;
            peak2 = peak1;
          }

          peak1 = now;
          peaks[peakCount] = refVal;

        } else if (isMin) {
          if (peakType == 0) { peakType=-1; }
          else if (peakType == 1) {
            peakType = -1;
            peakCount++;
            justchanged = true;
          }

          if (peakCount < 10) peaks[peakCount] = refVal;
        }

        //we've transitioned.  check if we can autotune based on the last peaks
        if (justchanged && peakCount > 2) {
          float avgSeparation = (abs(peaks[peakCount-1] - peaks[peakCount-2])
            + abs(peaks[peakCount-2] - peaks[peakCount-3])) / 2.0;

          if (avgSeparation < 0.05 * (absMax-absMin)) {
            serial::println::PGM(PSTR("PID autotune finished"));

            output(outputStart);
            m_tunning = false;

            const float Ku = 4 * (2 * oStep) / ((absMax-absMin) * M_PI);
            const float Tu = (float) (peak1 - peak2) / 1000.0;

            //serial::print::pair::float32(PSTR("Ku"), Ku, 2);
            //serial::print::pair::float32(PSTR("Pu"), Pu, 2);

            const float Wp = 0.6F   * Ku;
            const float Wi = 1.2F   * Ku / Tu;
            const float Wd = 0.075F * Ku * Tu;

            /*
            const float Wp = 0.6 * Ku;
            const float Wi = 0.5 * Tu;
            const float Wd = 0.125 * Tu;
            */

            serial::print::pair::float32(PSTR("Wp"), Wp, 2);
            serial::print::pair::float32(PSTR("Wi"), Wi, 2);
            serial::print::pair::float32(PSTR("Wd"), Wd, 2);

            //Kp(Wp); Ki(Wi); Kd(Wd);
            return;
          }
        }

        justchanged = false;
      }
    }
  }
}

void pid::irq() {
  if (! m_running || m_tunning) { return; }

  const float error = m_setpoint - m_input;         // calculate current error

  m_iError += error;                                // integrate the error
  if (m_iError > m_max) { m_iError = m_max; }       // cap the integral between
  else if (m_iError < m_min) { m_iError = m_min; }  // min and max values

  const float dError = m_input - m_last_input;      // calculate input derivative
  m_last_input = m_input;                           // save the input value

  // PID calculation
  float u = (m_Kp * error) + (m_Ki * m_iError) + (m_Kd * dError);

  #ifdef DEBUG
    serial::print::PGM(PSTR("setpoint: "));
    serial::print::float32(m_setpoint, 2);

    serial::print::PGM(PSTR(", input: "));
    serial::print::float32(m_input, 4);

    serial::print::PGM(PSTR(", error: "));
    serial::print::float32(error, 4);

    serial::print::PGM(PSTR(", iError: "));
    serial::print::float32(m_iError, 4);

    serial::print::PGM(PSTR(", last_input: "));
    serial::print::float32(m_last_input, 4);

    serial::print::PGM(PSTR(", dError: "));
    serial::print::float32(dError, 4);

    serial::print::PGM(PSTR(", P: "));
    serial::print::float32((m_Kp * error), 2);

    serial::print::PGM(PSTR(", I: "));
    serial::print::float32((m_Ki * m_iError), 2);

    serial::print::PGM(PSTR(", D: "));
    serial::print::float32((m_Kd * dError), 2);

    serial::print::PGM(PSTR(", u: "));
    serial::println::float32(u, 4);
  #endif

  if (u > m_max) { u = m_max; }       // cap the output between
  else if (u < m_min) { u = m_min; }  // min and max values

  m_output = u;
}

void pid::limit(const int16_t& min, const int16_t& max) {
  if (min > max) { return; }
  else { m_min = min, m_max = max; }
}

void pid::output(const float& lhs) {
  m_output = lhs;
  m_output = constrain(m_output, m_min, m_max);
}

void pid::reset() {
  m_iError = 0;
  m_last_input = 0;
}

void pid::tune(const float& Np, const float& Ni, const float& Nd) {
  // To use the PID into an inverted state i.e. output decreases with the
  // increase of error just provide negative paramaters.
  Kp(Np); Ki(Ni); Kd(Nd);
}
