# Testing checklist

## Hardware and firmware
- [ ] Confirm Uno is selected and the correct COM port is identified.
- [ ] Verify DS18B20 readings with a standalone temperature test before integrated testing.
- [ ] Confirm the 4.7 kΩ pull-up is correctly connected for a bare DS18B20.
- [ ] Verify pulse sensor raw output changes with sensor contact.
- [ ] Confirm Start short press starts monitoring.
- [ ] Confirm Start long press stops monitoring once per hold.
- [ ] Confirm Emergency short press emits `EMERGENCY_BUTTON` and activates buzzer.
- [ ] Confirm Emergency long press emits `RESET` and remains idle.
- [ ] Confirm the 1.5-second temperature-to-pulse gap and five pulse samples.
- [ ] Confirm 40.0 °C or above emits `STATUS:ABNORMAL_TEMP`.
- [ ] Confirm the configured pulse-estimate boundaries emit the expected status.

## Python and cloud
- [ ] Install packages from requirements.txt.
- [ ] Configure a local .env file with valid test credentials.
- [ ] Close Arduino Serial Monitor before launching Python.
- [ ] Confirm Python receives STARTED, STOPPED, RESET and status lines.
- [ ] Test SMS to an authorized/verified test recipient.
- [ ] Test voice call to an authorized/verified test recipient.
- [ ] Confirm duplicate emergency messages are suppressed while alert latch is active.
- [ ] Confirm STATUS:NORMAL or RESET clears the alert latch.

## Safety
- [ ] Use a test recipient who has agreed to receive test messages and calls.
- [ ] Do not use the prototype to monitor a real patient or make care decisions.
- [ ] Do not expose Twilio credentials in Git, screenshots, or public documentation.
