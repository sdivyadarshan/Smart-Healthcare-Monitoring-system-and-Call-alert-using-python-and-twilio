# Python alert application

The Python process opens the Arduino Uno's USB serial port, reads status lines, and sends Twilio SMS and voice-call requests when it receives an emergency event.

## Install

```bash
python -m pip install -r requirements.txt
```

Copy `config.example.env` to `.env`, then configure the serial port and Twilio credentials. Run `python main.py`.

Close Arduino Serial Monitor first. On Windows, identify the Uno's COM port in Device Manager or Arduino IDE.

**Credential safety:** `.env` is ignored by Git. Never paste a real Twilio Auth Token into source code, commits, screenshots, or chat. Rotate any token that has been exposed.

The alert latch is cleared by `STATUS:NORMAL` or `RESET`. For a real deployment, add delivery-status callbacks, retry/backoff policy, persistent event IDs, and a safer operator-controlled acknowledgement workflow.
