/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2018 João Brázio [joao@brazio.org]
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

#include "heater.h"

void Heater::eval_pid(const float &value)
{
  if(!m_automatic) return;

  // calculate compensated setpoint
  const int16_t target = ((m_config.setpoint + m_config.offset) * 10);
  const int16_t actual = value * 10;

  // calculate proportional error
  //const int16_t Perr = map((target - actual), 0, 5, 0, 255);
  const int16_t Perr = (target - actual);

  // calculate integral error
  const int32_t Itmp = (int32_t) m_runtime.Ierr + Perr;
  const int16_t Ierr = constrain(Itmp, (INT16_MIN +1), (INT16_MAX -1));

  // calculate derivative error
  const int16_t Derr = Perr - m_runtime.Perr;

  // calculate the PID
  const int16_t P = (m_config.Kp * Perr) *10 /100;
  const int16_t I = (m_config.Ki * Ierr) *10 /100;
  const int16_t D = (m_config.Kd * Derr) *10 /100;

  // calculate the output
  const int16_t u = P + I - D;

  // restrict the output
  m_value = (m_connected) ? constrain(u, m_config.min, m_config.max) : 0;

  // update runtime
  m_runtime.Ierr = Ierr;
  m_runtime.Derr = Derr;

  // only if debug active ?
  m_runtime.target = target;

  m_runtime.Perr = Perr;
  m_runtime.Ierr = Ierr;


  m_runtime.P = P;
  m_runtime.I = I;
  m_runtime.D = D;
  m_runtime.u = u;
}
