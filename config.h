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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define VERBOSE
#define BAUDRATE 57600

/**
 * User interface options
 */
#define UI_LCD_TYPE U8GLIB_SSD1306_128X64
#define UI_LCD_OPTIONS U8G_I2C_OPT_FAST
#define UI_LCD_ROTATION 180 // 90, 180, 270

//#define UI_ENCODER_ENABLE
#define UI_KEYPAD_ENABLE
#define UI_KEYPAD_A_PIN 8
#define UI_KEYPAD_B_PIN 12

// ----------------------------------------------------------------------------

//#define NOAA_DEW_FUNCTION

#define AMBIENT_SENSOR_DHT22
#define AMBIENT_PIN 7

#define STRIP_SENSOR1_PIN 4
#define STRIP_SENSOR2_PIN 2
//#define STRIP_SENSOR2_PIN 9
//#define STRIP_SENSOR2_PIN 9

#endif
