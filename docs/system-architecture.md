# System architecture

## 1. Sensing layer
The DS18B20 provides a digital temperature measurement over the OneWire bus. The pulse sensor provides an analog signal connected to A0.

## 2. Embedded processing layer
Arduino Uno reads the sensors, handles two active-low buttons, applies the configured demonstration thresholds, controls the buzzer, and emits serial event strings.

## 3. Guided communication layer
A USB cable provides power/programming and a wired serial data path between the Uno and laptop. This is not a wireless sensor network.

## 4. Python application layer
Python uses PySerial to receive and interpret Arduino messages. It uses environment-based configuration for the serial port and Twilio credentials.

## 5. Cloud notification layer
The Twilio REST API is accessed over the laptop's internet connection. Twilio handles SMS and outbound voice-call requests to the configured recipient.

## Data flow
Sensors → Arduino Uno → USB serial → Python on laptop → Internet → Twilio → emergency contact.
