![Image](https://github.com/jbrazio/arduheater/blob/master/doc/wiki/logo.png)

[![Donate](https://img.shields.io/badge/buy_me-one_beer-red.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D5XZ7QFLP8LXE) [![Build Status](https://travis-ci.org/jbrazio/arduheater.svg?branch=master)](https://travis-ci.org/jbrazio/arduheater) [![GitHub issues](https://img.shields.io/github/issues/jbrazio/arduheater.svg)](https://github.com/jbrazio/arduheater/issues) [![GitHub stars](https://img.shields.io/github/stars/jbrazio/arduheater.svg)](https://github.com/jbrazio/arduheater/stargazers) [![GitHub forks](https://img.shields.io/github/forks/jbrazio/arduheater.svg)](https://github.com/jbrazio/arduheater/network) [![Download beta](https://img.shields.io/badge/download-latest_beta-yellow.svg)](https://github.com/jbrazio/arduheater/archive/master.zip)


# Arduheater
Arduheater is a full open source intelligent heat strip controller for astronomy usage.


# Motivation
Controlling dew is a big thing if you really intent to do astronomy all year around.
This project will allow you to build a four channel intelligent heat strip contoller for less than 10€.


# Design Goals

## 1. Remotely controllable
This was a very important part of the design, most heat controllers, specially the DIY ones, rely on the PWM signal for each channel being manually adjusted by means of a potentiometer. This either requires the user to be near the device tweeking it or to set it to a temperature that may be higher than the one really needed thus completely trashing efficiency. Arduheater uses a serial connection so you can use any USB-Serial-TTL dongle to adjust it's settings either you're 2 or 2000 meters away.

## 2. Efficient energy usage
Manual adjusted heater will either require the user to be tweeking it or they will wast more energy than necessary due to the general tendency to use a higher that required setpoint, this is valid for any PWM or bang-bang style controllers. Arduheater uses a temperature sensor (DHT22) to measure basic environmental properties such as temperature and humidity, knowing them both makes the calculation of the dew point[1] possible. Arduheater also has a temperature sensor (NTC) for each heating strip, allowing the micro-controller to have a rough estimation of the temperature the equipment is at; it will be a rough estimation because we are really interested on the lenses surface temperature but we are actually measuring the heat strip temperature, to mitigate this, Arduheater allows the user to set specific offsets per heating strip.

Arduheater uses a PID controller[2] to efficiently manage the energy so only the required amount of energy to maintain a temperature setpoint is delivered to the heating strip. This is possible due the usage of a PWM signal while driving the outputs; the delta between the environmental dew point (plus offset, i.e. setpoint) and the heating strip temperature will make the micro-controller output a PID-calculated PWM signal until this delta reaches zero. In practical terms if a 12V heating strip full on consumes 12W of power (1A) it may be possible for it to use only 1W or even less to keep the equipment above dewpoint and the power usage will be automatically updated during the night as conditions vary, so the system will be always using the least amount of power to keep the dew away.

[1] Dew point is that dreadful threshold at which water condensation starts to happen on the lenses/equipment.

[2] A proportional–integral–derivative controller (PID controller) is a control loop feedback mechanism (controller) commonly used in industrial control systems.

## 3. Builder friendly
Using off-the-shelf components such as the Arduino Nano and easily available parts Arduheater is aimed to be build by anyone with a soldering iron and some patience, no degree in electronics required.

## 4. Multiple independent channels
Each of the four outputs have independent controls such as offset, min and max output power and of course the three main properties of the PID controller (Kp, Ki and Kd).



# Hardware
It was built on top of a standard ATmega 328 Arduino such as UNO, Pro or Nano; currently it does not support the Mega or any other ARM based board.

## BOM

- 25x Generic male pin header
- 1x Electrolytic Capacitor 10µF 20V
- 4x Rectifier Diode 3A
- 5x Screw Terminal (2 pin)
- 4x NPN Power Transistor TIP120
- 4x 1kΩ Resistor 5%
- 5x 10kΩ Resistor 1%
- 1x DHT22
- 1x Prefboard 5x7cm
- 4x GX12 Connector (4 pin)
- 1x Panel Fuse Holder
- 1x Fuse 5A

## 3D printed parts
To download and print instructions for the 3D printed parts have a look at the [Thing #2614341][2614341].

[2614341]: https://www.thingiverse.com/thing:2614341


## Schematic
The four diodes are not connected because they are optional since we're not dealing with high inductive loads.

![Schematic](https://github.com/jbrazio/arduheater/blob/master/doc/schematic-power_schem.jpg)

## Prototype board

![Prototype board](https://github.com/jbrazio/arduheater/blob/master/doc/schematic-power_bb.jpg)


# License
[GNU GPLv3](./LICENSE).

