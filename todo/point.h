/**
 * Arduheater - Telescope heat controller
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

#ifndef __POINT_T__
#define __POINT_T__

/**
 * @brief Cartesian Point
 * @details Represents a two dimensional point on Cartesian coordinate system.
 *
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 */
struct point_t {
  int16_t x;
  int16_t y;

  /**
   * @brief Two dimensional point constructor
   *
   * @param x The x-coordinate of the point.
   * @param y The y-coordinate of the point.
   */
  point_t(int16_t const &x, int16_t const &y) {
    this->x = x;
    this->y = y;
  }
};

#endif // __POINT_T__
