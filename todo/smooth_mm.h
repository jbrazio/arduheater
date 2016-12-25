/**
 * smooth_mm.h - Moving average smoothing algorithm
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

template <typename T, uint8_t N>
struct mmsmooth_t {
private:
  T sum;
  T values[N];
  uint8_t index;
  uint8_t readings;

public:
  mmsmooth_t() {
    this->init();
  }

  void init() {
    this->sum = 0;
    this->index = 0;
    this->readings = 1;
    for (uint8_t i = 0; i < N; i++) this->values[i] = 0;
  }

  /*
  inline bool operator< (const T &value) {
    if (this.get() < value) return true;
    else return false;
  }

  inline bool operator==(const T &value) {
    if (this->get() == value) return true;
    else return false;
  }

  inline bool operator> (const T &value) { return value < this->get(); }
  inline bool operator<=(const T &value) { return !(this->get() > value); }
  inline bool operator>=(const T &value) { return !(this->get() < value); }
  inline bool operator!=(const T &value) { return !(this->get() == value); }
  */

  inline mmsmooth_t& operator=(const T &value) {
    this->init();
    this->values[0] = value;
    this->sum += value;
    this->index = 1;

    return *this;
  }

  inline mmsmooth_t& operator+=(const T &value) {
    if (isnan(value)) return *this;

    this->sum -= this->values[this->index];
    this->values[this->index] = value;
    this->sum += value;
    this->index++;

    if (this->index > (N -1)) this->index = 0;
    if (this->readings < (N +1)) this->readings++;

    return *this;
  }

  inline const T get() {
    if (this->readings == 1) return 0;
    return this->sum / (this->readings -1);
  }
};

#endif
