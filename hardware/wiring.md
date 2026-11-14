# Wiring and pin connections

## Arduino Uno pin map

| Module / signal | Arduino Uno |
|---|---|
| Pulse sensor analog output | A0 |
| DS18B20 data | D2 |
| Start button | D3 to GND |
| Emergency button | D4 to GND |
| Buzzer signal | D5 |
| DS18B20 VDD | 5V (check module specification) |
| DS18B20 GND | GND |
| Pulse sensor VCC / GND | Supply / GND per module specification |

Both buttons use `INPUT_PULLUP`; connect each button between its assigned digital pin and GND. A pressed button therefore reads LOW.

## DS18B20 pull-up

For a bare three-wire DS18B20, connect a 4.7 kΩ resistor between DATA (D2) and 5V. Typical TO-92 pin order, when viewing the flat face, is GND, DATA, VDD, but verify the datasheet for the exact part. Waterproof probes and breakout modules may use different wire colors or include the resistor already.

## Notes

- Connect all grounds to a common Arduino GND.
- Do not connect a buzzer or sensor beyond the current rating of the Uno pins; use a transistor driver for loads requiring more current.
- Disconnect power before changing wiring.
- The Arduino connects to the laptop over USB; there is no ESP32 or Wi-Fi hardware in this design.
