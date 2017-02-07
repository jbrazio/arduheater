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

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "state.h"

/**
 * Hardware abstraction class
 */
template <typename T>
class Hardware {
protected:
  volatile STATE_t sensorState;
  volatile uint32_t lastReadTime;
  const uint32_t refreshTime = 4999L;

public:
  /**
   * Class constructor
   */
  Hardware() {
    this->sensorState = STATE_UNKNOWN;
    this->lastReadTime = 0;
  }

  /**
   * Performs all the required actions to initialize the hardware and
   * the sensorState value should be updated accordingly, STATE_READY
   * signals a successfuly hardware initialization.
   * @return boolean value indicating the success of the process
   */
  virtual bool setup() {
    return true;
  }

  /**
   * [reset description]
   * @return [description]
   */
  virtual bool reset() {
    this->sensorState = STATE_READY;
    return true;
  }

  /**
   * This method is not only responsible for returning the sensorState
   * flag and updating it when the current state is STATE_SLEEP.
   * @return STATE_t value representing the current state of the sensor
   */
  virtual STATE_t state() {
    // Keeps the refresh rate under the hardware limit
    if (this->sensorState == STATE_SLEEP
    && (uint32_t) (millis() - this->lastReadTime) > this->refreshTime) {
      this->sensorState = STATE_READY;
    }

    return this->sensorState;
  }

  /**
   * [refresh description]
   * @return [description]
   */
  virtual bool refresh() = 0;

  /**
   * [get description]
   * @return [description]
   */
  virtual T get() = 0;

  /**
   * [get description]
   * @param  what [description]
   * @return      [description]
   */
  virtual T get(const uint8_t &what) = 0;
};

#endif
