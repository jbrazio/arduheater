/**
 * movingmean.h - Moving mean smoothing algorithm
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

#ifndef __SMOOTHING_MM_H__
#define __SMOOTHING_MM_H__

#include "arduheater.h"

template <typename T, uint8_t N> struct movingmean
{
public:
  movingmean() { reset(); }

private:
  T       m_values[N];
  T       m_sum_of_values;
  uint8_t m_index;
  uint8_t m_count;

private:
  inline void reset() {
    m_count = m_index = m_sum_of_values = 0;
    for (size_t i = 0; i < N; i++) { m_values[i] = 0; }
  }

public:
  inline bool full() {
    return (m_count == N);
  }

  inline T operator()() {
    if (m_count == 0) { return 0; }
    return m_sum_of_values / m_count;
  }

  inline movingmean& operator=(const T &lhs) {
    reset();
    m_count = m_index = 1;
    m_values[0]       = lhs;
    m_sum_of_values   = lhs;
    return (*this);
  }

  inline movingmean& operator+=(const T &lhs) {
    m_sum_of_values    -= m_values[m_index];
    m_values[m_index++] = lhs;
    m_sum_of_values    += lhs;

    if (m_index >= N) { m_index = 0; }
    else if (m_count <  N) { ++m_count; }

    return (*this);
  }
};

#endif
