"""Laptop-side serial listener and Twilio emergency notifier.

Configure credentials in a local .env file. Never commit that file.
"""
import os
import sys
import time

import serial
from dotenv import load_dotenv
from twilio.rest import Client
from twilio.base.exceptions import TwilioRestException

load_dotenv()

SERIAL_PORT = os.getenv("SERIAL_PORT", "COM5")
BAUD_RATE = int(os.getenv("BAUD_RATE", "9600"))

ACCOUNT_SID = os.getenv("TWILIO_ACCOUNT_SID")
AUTH_TOKEN = os.getenv("TWILIO_AUTH_TOKEN")
FROM_NUMBER = os.getenv("TWILIO_FROM_NUMBER")
TO_NUMBER = os.getenv("ALERT_TO_NUMBER")

if not all([ACCOUNT_SID, AUTH_TOKEN, FROM_NUMBER, TO_NUMBER]):
    sys.exit("Missing Twilio configuration. Copy config.example.env to .env and fill it in.")

twilio = Client(ACCOUNT_SID, AUTH_TOKEN)
alert_active = False


def send_emergency_alert(message: str) -> None:
    """Send one SMS and one voice call for the current emergency episode."""
    global alert_active
    if alert_active:
        print("[ALERT] An alert is already active; duplicate notification suppressed.")
        return

    # Set only after both API requests are attempted successfully enough to avoid
    # an accidental repeated alert loop. Exceptions are reported to the operator.
    print("[ALERT] Sending SMS...")
    sms = twilio.messages.create(
        body=message,
        from_=FROM_NUMBER,
        to=TO_NUMBER,
    )
    print(f"[ALERT] SMS accepted by Twilio: {sms.sid}")

    print("[ALERT] Initiating voice call...")
    call = twilio.calls.create(
        twiml=(
            "<Response><Say language=\"en-US\">"
            + message
            + "</Say></Response>"
        ),
        from_=FROM_NUMBER,
        to=TO_NUMBER,
    )
    print(f"[ALERT] Call request accepted by Twilio: {call.sid}")
    alert_active = True


def handle_line(line: str) -> None:
    global alert_active
    print(f"[ARDUINO] {line}")

    if line == "STATUS:ABNORMAL_TEMP":
        send_emergency_alert(
            "Emergency alert. The healthcare monitoring prototype detected "
            "a temperature at or above its configured 40 degree Celsius threshold."
        )
    elif line == "STATUS:ABNORMAL_HR":
        send_emergency_alert(
            "Emergency alert. The healthcare monitoring prototype detected "
            "a pulse estimate outside its configured range."
        )
    elif line == "EMERGENCY_BUTTON":
        send_emergency_alert(
            "Manual emergency alert. The emergency button on the healthcare "
            "monitoring prototype was pressed."
        )
    elif line == "STATUS:NORMAL" or line == "RESET":
        alert_active = False
        print("[SYSTEM] Alert latch cleared.")
    elif line == "STOPPED":
        print("[SYSTEM] Monitoring stopped.")


def main() -> None:
    print(f"Opening Arduino serial port {SERIAL_PORT} at {BAUD_RATE} baud...")
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as connection:
            time.sleep(2)  # Allow the Uno to reset after opening the serial port.
            print("Connected. Press Ctrl+C to exit.")
            while True:
                raw = connection.readline()
                if not raw:
                    continue
                line = raw.decode("utf-8", errors="replace").strip()
                if not line:
                    continue
                try:
                    handle_line(line)
                except TwilioRestException as exc:
                    print(f"[TWILIO ERROR] {exc}")
                except Exception as exc:
                    print(f"[ERROR] Could not process event: {exc}")
    except serial.SerialException as exc:
        sys.exit(f"Serial connection failed: {exc}")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nExiting monitoring application.")
