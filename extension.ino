#include <CapacitiveSensor.h>

// Capacitive touch sensor
CapacitiveSensor touchSensor(4, 6);

// Pins
const int trackPowerPin = 2;
const int probePin = A0;
const int buzzerPin = 9;

// Capacitive touch threshold
const long touchThreshold = 900;

// Graphite strip settings
const int silenceThreshold = 15;
const int minPitch = 150;
const int maxPitch = 1000;

// Octave control
bool octaveHigh = false;
bool wasTouched = false;

void setup() {
  pinMode(trackPowerPin, OUTPUT);
  digitalWrite(trackPowerPin, LOW);

  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);

  // Disable auto-calibration
  touchSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);
}

void loop() {

  // Read capacitive touch sensor
  long touchValue = touchSensor.capacitiveSensor(30);

  // Detect a new touch and toggle octave
  bool touched = (touchValue > touchThreshold);

  if (touched && !wasTouched) {
    octaveHigh = !octaveHigh;

    Serial.print("Octave: ");
    if (octaveHigh) {
      Serial.println("HIGH");
    } else {
      Serial.println("LOW");
    }
  }

  wasTouched = touched;

  // Read graphite strip
  digitalWrite(trackPowerPin, HIGH);
  delayMicroseconds(1000);

  analogRead(probePin);  // discard first reading
  int reading = analogRead(probePin);

  digitalWrite(trackPowerPin, LOW);

  Serial.print("Touch = ");
  Serial.print(touchValue);
  Serial.print("   Reading = ");
  Serial.println(reading);

  if (reading <= silenceThreshold) {
    noTone(buzzerPin);
  }
  else {
    int pitch = map(
      reading,
      silenceThreshold + 1,
      1023,
      minPitch,
      maxPitch
    );

    // Shift up one octave when octaveHigh is enabled
    if (octaveHigh) {
      pitch *= 2;
    }

    tone(buzzerPin, pitch);
  }

  delay(10);
}
