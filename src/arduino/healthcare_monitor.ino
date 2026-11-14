#include <OneWire.h>
#include <DallasTemperature.h>

#define PULSE_PIN A0
#define TEMP_PIN 2
#define START_BTN 3
#define EMG_BTN 4
#define BUZZER 5

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

bool monitoring = false;
bool emergencyMode = false;

unsigned long startPressTime = 0;
unsigned long emgPressTime = 0;
bool startHeld = false;
bool emgHeld = false;
bool startLongActionDone = false;
bool emgLongActionDone = false;

void clickBeep() {
  digitalWrite(BUZZER, HIGH);
  delay(80);
  digitalWrite(BUZZER, LOW);
}

void beepTwice() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
}

void emergencyOn() {
  digitalWrite(BUZZER, HIGH);
}

void emergencyOff() {
  digitalWrite(BUZZER, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(EMG_BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  sensors.begin();
  Serial.println("SYSTEM_READY");
}

void loop() {
  // START: short press starts; long press stops.
  if (digitalRead(START_BTN) == LOW) {
    if (!startHeld) {
      clickBeep();
      startPressTime = millis();
      startHeld = true;
      startLongActionDone = false;
    }

    if (!startLongActionDone && millis() - startPressTime >= 3000) {
      monitoring = false;
      emergencyMode = false;
      emergencyOff();
      Serial.println("STOPPED");
      beepTwice();
      startLongActionDone = true;
    }
  } else {
    if (startHeld && !startLongActionDone &&
        millis() - startPressTime < 3000 && !monitoring) {
      monitoring = true;
      emergencyMode = false;
      Serial.println("STARTED");
      beepTwice();
    }
    startHeld = false;
  }

  // EMERGENCY: short press activates; long press resets and returns to idle.
  if (digitalRead(EMG_BTN) == LOW) {
    if (!emgHeld) {
      clickBeep();
      emgPressTime = millis();
      emgHeld = true;
      emgLongActionDone = false;
    }

    if (!emgLongActionDone && millis() - emgPressTime >= 3000) {
      emergencyMode = false;
      monitoring = false;
      emergencyOff();
      Serial.println("RESET");
      beepTwice();
      emgLongActionDone = true;
    }
  } else {
    if (emgHeld && !emgLongActionDone &&
        millis() - emgPressTime < 3000) {
      emergencyMode = true;
      monitoring = false;
      Serial.println("EMERGENCY_BUTTON");
      emergencyOn();
    }
    emgHeld = false;
  }

  // Monitoring cycle. DS18B20 uses its normal conversion interval.
  if (monitoring && !emergencyMode) {
    Serial.println("----- Checking Temperature -----");
    sensors.requestTemperatures();
    delay(800);

    float temperature = sensors.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C ");

    if (temperature == DEVICE_DISCONNECTED_C) {
      Serial.println("(SENSOR_ERROR)");
      Serial.println("STATUS:SENSOR_ERROR");
      monitoring = false;
      emergencyMode = true;
      emergencyOn();
    } else if (temperature >= 40.0) {
      Serial.println("(ABNORMAL)");
      Serial.println("STATUS:ABNORMAL_TEMP");
      monitoring = false;
      emergencyMode = true;
      emergencyOn();
    } else {
      Serial.println("(NORMAL)");
      delay(1500);

      Serial.println("----- Checking Pulse -----");
      long pulseSum = 0;
      for (int i = 0; i < 5; i++) {
        pulseSum += analogRead(PULSE_PIN);
        delay(100);
      }

      int averagePulse = pulseSum / 5;
      int estimatedBpm = map(averagePulse, 0, 1023, 50, 120);

      Serial.print("Pulse estimate: ");
      Serial.print(estimatedBpm);
      Serial.print(" BPM ");

      if (estimatedBpm < 50 || estimatedBpm > 120) {
        Serial.println("(ABNORMAL)");
        Serial.println("STATUS:ABNORMAL_HR");
        monitoring = false;
        emergencyMode = true;
        emergencyOn();
      } else {
        Serial.println("(NORMAL)");
        Serial.println("STATUS:NORMAL");
        beepTwice();
      }
    }

    Serial.println("------------------------------");
    delay(3000);
  }
}
