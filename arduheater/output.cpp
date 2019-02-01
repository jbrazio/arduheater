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

#include "output.h"

/**
 * @brief Preinstantiate objects
 */
Output output[4] = {
  Output(0), Output(1), Output(2), Output(3)
};

void Output::callback(const uint8_t& channel, const uint16_t& value)
{
  // string buffer for the serial messages bellow
  char buffer[33];

  if(m_channel != channel) {
    sprintf_P(buffer, PSTR("I Wrong channel (%i) instance (%i)."), channel +1, m_channel);
    LogLn::string(buffer);
    return;
  }

  // store the averaged raw adc value
  set_sensor_value(value);

  // calculate the edges which will make the raw value report the NTC as disconnected
  const static uint16_t min_edge = (0    + NTC_ERR_RAW_THRESHOLD);
  const static uint16_t max_edge = (1023 - NTC_ERR_RAW_THRESHOLD);

  // When no sensor is connected to the ADC channel the resistor on the voltage
  // divider will act as a pull-up resistor thus the ADC value will be 1023.
  if(value < min_edge || value > max_edge) {
    if(is_connected()) {
      sprintf_P(buffer, PSTR("I Thermistor-%i disconnected."), channel +1);
      LogLn::string(buffer);
    }

    disable();
    connected(false);
  }

  else {
    // No matter if the sensor returns valid data or not, we mark it as
    // physically connected.
    if(!is_connected()) {
      sprintf_P(buffer, PSTR("I Thermistor-%i connected."), channel +1);
      LogLn::string(buffer);
    }

    connected(true);

    // Check if the data returned by the sensor falls under our expected range
    // if it does not then we mark the sensor as not ready.
    if(value < NTC_MIN_RAW_VALUE || value > NTC_MAX_RAW_VALUE) {
      if(is_ready()) {
        sprintf_P(buffer, PSTR("W Thermistor-%i reading error."), channel +1);
        LogLn::string(buffer);
      }

      ready(false);

      // If the heater is active we will wait NTC_MAX_ERRORS before powering off,
      // this allows temporary self-healing errors to occur.
      static uint8_t error_counter = 0;
      if(error_counter > NTC_MAX_ERRORS) {
        if(is_enabled()) {
          sprintf_P(buffer, PSTR("E Output-%i disabled, thermistor not ready."), channel +1);
          LogLn::string(buffer);
        }

        error_counter = 0;
        disable();

      } else { ++error_counter; }
    }

    else { ready(true); }
  }
}

void Output::eval_pid()
{
  if(!m_automatic) { return; }
  if(!is_connected()) { disable(); return; }

  const float actual = temperature();

  const float dt = 2;

  // calculate compensated setpoint (+offset)
  const float target = setpoint();

  // calculate the error
  const float error = (target - actual);

  // calculate the proportional term
  const float Pterm = m_config.Kp * error;

  // integrate the error
  float Ierror = (error * dt) + m_runtime.Ierror;

  // limit the integration error
  Ierror = constrain(Ierror, -255, 255);

  // calculate the integral term
  const float Iterm  = m_config.Ki * Ierror;

  // calculate the derivative term
  const float Dterm = m_config.Kd * ((error - m_runtime.Lerror) / dt);

  // calculate the output
  const int16_t u = (Pterm + Iterm + Dterm);
  set_output_value(constrain(u, m_config.min, m_config.max));

  // update runtime
  m_runtime.Ierror = Ierror;
  m_runtime.Lerror = error;

  m_runtime.Pterm = Pterm;
  m_runtime.Iterm = Iterm;
  m_runtime.Dterm = Dterm;
  m_runtime.u = u;
}

float Output::temperature(const bool &calibrated)
{
  if(!is_connected()) { return 0; }

  // steinhart
  float temp = constrain(m_sensor_value, 1, 1022);
  temp  = m_config_ntc.resistor / (1023.0F / temp - 1); // convert raw to ohms
  temp /= m_config_ntc.nominalval;                      // (R/Ro)
  temp  = log(temp);                                    // ln(R/Ro)
  temp /= m_config_ntc.bcoefficient;                    // 1/B * ln(R/Ro)
  temp += 1.0F / (m_config_ntc.nominaltemp + 273.15F);  // + (1/To)
  temp  = 1.0F / temp;                                  // invert
  temp -= 273.15F;                                      // convert to K to C

  return (calibrated) ? (temp + m_config.temp_offset) : (temp);
}

uint16_t Output::invtemp(const float& temp)
{
  float value = temp;
  value += 273.15F;
  value  = 1.0F / value;
  value -= 1.0F / (m_config_ntc.nominaltemp + 273.15F);
  value *= m_config_ntc.bcoefficient;
  value  = exp(value);
  value *= m_config_ntc.nominalval;
  value  = (1023.0F * value) / (value + m_config_ntc.resistor);
  return (uint16_t) value;
}
