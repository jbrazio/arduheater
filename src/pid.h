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

#ifndef __PID_H__
#define __PID_H__

#include "arduheater.h"

class pid
{
public:
  pid() {
    limit(0, 255);
    mode(pid::MANUAL);
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
  float output();
  pid::mode_t mode();
  void input(const float& lhs);
  void irq(const bool& reset = false);
  void limit(const float& min, const float& max);
  void mode(const pid::mode_t& lhs);
  void output(const float& lhs);
  void sampletime(const float& lhs);
  void setpoint(const float& lhs);
  void tune(const float& Kp, const float& Ki, const float& Kd);

  void autotune(const uint8_t& thermistor, const uint8_t& temp, const uint8_t& cycles);
};

#endif
