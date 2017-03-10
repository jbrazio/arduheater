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

// This PID lib is heavly based on the Arduino's PID library by Brett and his
// excelent tutorial published at http://brettbeauregard.com, thus some code
// is (c) 2004 Brett Beauregard <br3ttb@gmail.com>

#ifndef __PID_H__
#define __PID_H__

#include "common.h"

class PID
{
public:
  PID() {
    limit(0, 255);
    mode(PID::MANUAL);
    sampletime(HEARTBEAT);
  }

public:
  enum mode_t { MANUAL, AUTOMATIC };

protected:
  float m_input, m_output, m_setpoint;
  float m_Kp, m_Ki, m_Kd, m_dt;
  float m_min, m_max;

protected:
  mode_t m_mode;

public:
  void irq(const bool& reset = false) {
    asm volatile ("/************ PID::irq() ************/");
    if (m_mode != PID::AUTOMATIC) { return; }

    //byte * b;
    //serial::write(67);

    //b = (byte *) &m_setpoint;
    //for (size_t i = 0; i < 4; i++ ) serial::write(b[i]);

    //b = (byte *) &m_input;
    //for (size_t i = 0; i < 4; i++ ) serial::write(b[i]);

    static float s_last_input = 0;            // input value from last cycle
    float error  = m_setpoint - m_input;      // calculate current error
    float dInput = m_input - s_last_input;    // calculate input derivative

    //b = (byte *) &error;
    //for (size_t i = 0; i < 4; i++ ) serial::write(b[i]);

    static float s_error = 0;         // integration of error from 0 to present
    s_error += m_Ki * (error * m_dt); // adding the Ki term at this point will
                                      // allow a smooth curve when tuning while
                                      // running.

    if (reset) { s_error = m_output; dInput = 0; } // reboot the algorithm

    if (s_error > m_max) { s_error = m_max; }       // cap the I term between
    else if (s_error < m_min) { s_error = m_min; }  // min and max values

    //b = (byte *) &s_error;
    //for (size_t i = 0; i < 4; i++ ) serial::write(b[i]);

    // evaluate the PID algorithm
    float u = (m_Kp * error) + s_error - (m_Kd * (dInput / m_dt));

    if (u > m_max) { u = m_max; }       // cap the output between
    else if (u < m_min) { u = m_min; }  // min and max values

    //b = (byte *) &u;
    //for (size_t i = 0; i < 4; i++ ) serial::write(b[i]);

    m_output = u;
    s_last_input = m_input;
    asm volatile ("/************ PID::irq() ************/");
  }

  void limit(const float& min, const float& max) {
    if (min > max) { return; }
    m_min = min; m_max = max;
  }

  inline PID::mode_t mode() {
    return m_mode;
  }

  inline void mode(const PID::mode_t& lhs) {
    // force algorithm reboot if going into automatic mode from manual mode
    if (lhs == PID::AUTOMATIC && m_mode == PID::MANUAL) { irq(true); }
    m_mode = lhs;
  }

  void tune(const float& Kp, const float& Ki, const float& Kd) {
    // To use the PID into an inverted state i.e. output decreases with the
    // increase of error just provide negative paramaters.
    m_Kp = Kp; m_Ki = Ki; m_Kd = Kd;
  }

  inline void output(const float& lhs) {
    if (m_mode != PID::MANUAL) { return; }
    m_output = lhs;
  }

  inline float output()                     { return m_output; }
  inline void input(const float& lhs)       { m_input = lhs; }
  inline void sampletime(const float& lhs)  { m_dt = lhs / 1000L; }
  inline void setpoint(const float& lhs)    { m_setpoint = lhs; }
};

#endif
