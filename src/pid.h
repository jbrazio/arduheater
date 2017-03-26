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
    stop();
    limit(0, 255);
    sampletime(HEARTBEAT);
    m_running = m_tunning = false;
  }

protected:
  bool m_running, m_tunning;
  float m_Kp, m_Ki, m_Kd, m_dt;
  float m_input, m_output, m_setpoint;
  float m_min, m_max;

public:
  void autotune();
  void input(const float& lhs) { m_input = lhs; }
  void irq(const bool& reset = false);
  void limit(const float& min, const float& max);
  void output(const float& lhs);
  void tune(const float& Np, const float& Ni, const float& Nd);

public:
  inline bool  active()                     { return m_running;   }
  inline float Kd()                         { return m_Kd;        }
  inline float Ki()                         { return m_Ki;        }
  inline float Kp()                         { return m_Kp;        }
  inline float output()                     { return m_output;    }
  inline float setpoint()                   { return m_setpoint;  }
  inline void  Kd(const float& lhs)         { m_Kd = lhs;         }
  inline void  Ki(const float& lhs)         { m_Ki = lhs;         }
  inline void  Kp(const float& lhs)         { m_Kp = lhs;         }
  inline void  sampletime(const float& lhs) { m_dt = lhs / 1000L; }
  inline void  setpoint(const float& lhs)   { m_setpoint = lhs;   }
  inline void  start()                      { m_running = true;   }
  inline void  stop()                       { m_running = false;  }
};

#endif
