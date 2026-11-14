# Algorithm

1. Initialize serial communication, button inputs, buzzer output, and the DS18B20 library.
2. Wait in idle state until the Start button is pressed briefly.
3. A long Start-button press (3 seconds) stops monitoring.
4. During each monitoring cycle, request a DS18B20 conversion and wait approximately 800 ms before reading.
5. If the temperature sensor is disconnected, emit `STATUS:SENSOR_ERROR` and enter a local fault/emergency state.
6. If temperature is at least 40.0 °C, emit `STATUS:ABNORMAL_TEMP`, stop monitoring, and activate the buzzer.
7. Otherwise wait 1.5 seconds, take five analog pulse-sensor samples at 100 ms intervals, and average the raw values.
8. Map the average analog value to the prototype's approximate 50–120 BPM display range. This is a demonstration mapping, not validated pulse detection.
9. If the estimate is below 50 or above 120, emit `STATUS:ABNORMAL_HR` and activate the emergency state. Otherwise emit `STATUS:NORMAL`.
10. A short Emergency-button press emits `EMERGENCY_BUTTON`. A long press (3 seconds) emits `RESET` and returns to idle.
11. Python reads serial lines. For abnormal status or manual emergency, it requests an SMS and voice call from Twilio. A normal status or reset clears the Python alert latch.

The sketch uses blocking delays during sensor conversion, sampling, and buzzer feedback. Button response is therefore not instantaneous while a monitoring cycle is in progress.
