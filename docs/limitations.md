# Limitations and safety

- This is an educational prototype and is not a medical device.
- The DS18B20 measures the temperature at its probe location. Skin/surface measurements are not equivalent to validated core body temperature.
- The current pulse code averages five ADC readings and maps the result to an approximate numeric range. It does not detect pulse peaks, calculate inter-beat intervals, or validate BPM.
- Thresholds are demonstration values and are not individualized clinical limits.
- USB connection, an active laptop, a running Python process, internet access, and a functioning Twilio account are required for cloud alerts.
- Twilio delivery and call setup depend on service availability, account status, recipient verification, carrier routing, and regional regulations.
- Blocking delays can postpone button handling.
- No watchdog, battery backup, redundant communications path, authenticated device protocol, or clinical validation is implemented.

Never rely on this prototype for diagnosis, treatment, or real emergency response. Use approved medical devices and local emergency services where needed.
