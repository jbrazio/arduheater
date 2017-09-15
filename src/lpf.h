/**
 * lpf.h - Moving mean smoothing algorithm
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

#ifndef __SMOOTHING_LPF_H__
#define __SMOOTHING_LPF_H__

#include "arduheater.h"

template <typename T, uint16_t N> struct lpf
{
private:
  bool m_init;
  volatile T m_smooth_data;
  const float k_beta = (N / 1000.0F);

public:
  inline T operator()() const {
    return m_smooth_data;
  }

  inline lpf& operator=(const T& lhs) {
    m_smooth_data = lhs;
    m_init = true;
    return (*this);
  }

  inline lpf& operator+=(const T& lhs) {
    if (! m_init) {
      m_smooth_data = lhs;
      m_init = true;
    }

    else {
      m_smooth_data = m_smooth_data - (k_beta * (m_smooth_data - lhs));
    }

    return (*this);
  }
};

#endif
