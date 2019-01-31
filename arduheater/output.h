/**
 * Arduheater - An intelligent dew buster for astronomy
 * Copyright (C) 2016-2019 João Brázio [joao@brazio.org]
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

#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <stdint.h>
#include <stdlib.h>

#include "version.h"
#include "config.h"

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "macro.h"
#include "type.h"
#include "utility.h"

/**
 * @brief Heater structure
 * @details Top level heater structure block.
 *
 */
class Output
{
  public:
    typedef struct {
      uint8_t min, max;
      bool    autostart;
      float   temp_offset, setpoint_offset, Kp, Ki, Kd;
    } config_t;

    typedef struct {
      float    nominaltemp;
      uint16_t resistor, bcoefficient, nominalval;
    } config_ntc_t;

    typedef struct {
      volatile float Pterm, Iterm, Dterm, u;
      volatile float Ierror, Lerror;
    } runtime_t;

  protected:
    config_t m_config = {
      0, 255, false, 0.0F, 0.0F, DEFAULT_Kp, DEFAULT_Ki, DEFAULT_Kd
    };

    config_ntc_t m_config_ntc {
      nominaltemp: DEFAULT_NTC_NT, resistor: DEFAULT_NTC_SR,
      bcoefficient: DEFAULT_NTC_BC, nominalval: DEFAULT_NTC_NV
    };

    runtime_t m_runtime = { 0, 0, 0, 0, 0, 0 };

    // TODO
    // Currently this is not used, the idea is to allow the user to manually set
    // heating power.
    volatile bool m_automatic = false;

    // Target temperature we want to follow
    volatile float m_setpoint;

    // Holds the current PWM value for the output pin, we should never write directly
    // to the pin has the ISR will constantly enforce this value on the output.
    volatile uint8_t m_output_value = 0;

    // Holds the thermistor ADC value
    volatile uint16_t m_sensor_value = 1023;

    // Allows to pair each Heater with a Thermistor.
    uint8_t m_channel = 0;

    // Physically it's not possible to tell if the heater element of an output is
    // connected, nevertheless we will use this property logically. An heater will
    // be "connected" when the corresponding thermistor is connected, it will be
    // "disconnected" otherwise.
    volatile bool m_connected = false;

    // The sensor could be connected but the data returned by it maybe garbage, this
    // flag indicated either we are getting good data or bad data. A "connected"
    // sensor returning good data is considered "ready".
    volatile bool m_ready = false;

  public:
    Output(const uint8_t& channel) {
      m_channel = channel;
    }

  public:
    inline bool is_connected()                              { return (m_connected); }
    inline void connected(const bool &value)                { m_connected = value;  }

    inline bool is_enabled()                                { return m_automatic;                            }
    inline void enable()                                    { m_automatic = (is_connected()) ? true : false; }
    inline void disable()                                   {
      m_output_value = 0;
      m_automatic = false;
      memset(&m_runtime, 0, sizeof(runtime_t));
    }

    inline void  set_setpoint(const float &value)           { m_setpoint = value; }
    inline float setpoint(const bool &raw = false)          {
      return (raw) ? m_setpoint : (m_setpoint + m_config.setpoint_offset);
    }

    inline void    set_output_value(const uint8_t &value)   { m_output_value = value;   }
    inline uint8_t output_value()                           { return (m_output_value);  }

    inline bool is_autostart()                              { return (m_config.autostart);  }
    inline void autostart(const bool &value)                {  m_config.autostart = value;  }

    inline void  set_temp_offset(const float &value)        { m_config.temp_offset = value; }
    inline float temp_offset()                              { return m_config.temp_offset;  }

    inline void  set_setpoint_offset(const float &value)    { m_config.setpoint_offset = value; }
    inline float setpoint_offset()                          { return m_config.setpoint_offset;  }


    inline void    set_min_output(const uint8_t &value)     { m_config.min = value; }
    inline uint8_t min_output()                             { return m_config.min;  }

    inline void    set_max_output(const uint8_t &value)     { m_config.max = value; }
    inline uint8_t max_output()                             { return m_config.max;  }

    inline void  set_kp(const float &value)                 { m_config.Kp = value; }
    inline float kp()                                       { return m_config.Kp;  }

    inline void  set_ki(const float &value)                 { m_config.Ki = value; }
    inline float ki()                                       { return m_config.Ki;  }

    inline void  set_kd(const float &value)                 { m_config.Kd = value; }
    inline float kd()                                       { return m_config.Kd;  }

    inline uint8_t channel()                                { return m_channel; }

    void eval_pid();
    void callback(const uint8_t&, const uint16_t&);

  public:
    inline bool is_ready()                                        { return (m_ready); }
    inline void ready(const bool &value)                          { m_ready = value;  }

    inline void       set_sensor_value(const float &value)        { m_sensor_value = value; }
    inline uint16_t   sensor_value()                              { return m_sensor_value;  }

    inline void  set_nominal_temp(const float &value)             { m_config_ntc.nominaltemp = value; }
    inline float nominal_temp()                                   { return m_config_ntc.nominaltemp;  }

    inline void     set_resistor_value(const uint16_t &value)     { m_config_ntc.resistor = value; }
    inline uint16_t resistor_value()                              { return m_config_ntc.resistor;  }

    inline void     set_bcoefficient_value(const uint16_t &value) { m_config_ntc.bcoefficient = value; }
    inline uint16_t bcoefficient_value()                          { return m_config_ntc.bcoefficient;  }

    inline void     set_nominal_value(const uint16_t &value)      { m_config_ntc.nominalval = value; }
    inline uint16_t nominal_value()                               { return m_config_ntc.nominalval;  }

    float temperature(const bool &raw = false);
    uint16_t invtemp(const float&);

  public:
    inline runtime_t&    export_runtime()    { return m_runtime;    }
    inline config_t&     export_config()     { return m_config;     }
    inline config_ntc_t& export_config_ntc() { return m_config_ntc; }

    inline void import_config(config_t config)         { m_config = config; }
    inline void import_config_ntc(config_ntc_t config) { m_config_ntc = config; }
};

extern Output output[4];

#endif
