# Design decisions

- **Arduino Uno selected:** The project uses the available Uno as the embedded controller; no ESP32 or onboard Wi-Fi is used.
- **Laptop-hosted Python:** The laptop runs the Python process and provides internet connectivity for cloud calling. This keeps cloud credentials out of the Arduino firmware.
- **USB serial:** A guided wired link was chosen for predictable bench testing and straightforward debugging.
- **DS18B20:** A digital temperature sensor was selected. A 4.7 kΩ data pull-up is used for a bare sensor configuration.
- **Two-button interface:** Start supports short-press start and long-press stop. Emergency supports short-press manual alert and long-press reset.
- **Local buzzer:** Audible feedback remains available even before the cloud notification is delivered.
- **Thresholds:** 40 °C and the approximate pulse range 50–120 are prototype rules selected for demonstration, not clinical criteria.
- **Five pulse samples:** The current sketch averages five raw ADC readings. This smooths raw input slightly but does not calculate heart rate from detected beats.
- **Twilio:** Cloud SMS and voice calls are initiated by Python rather than the Uno.
