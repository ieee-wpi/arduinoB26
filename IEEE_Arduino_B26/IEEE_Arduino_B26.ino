const int trackPowerPin = 2;
const int probePin = A0;
const int buzzerPin = 9;

// Adjust these after testing your graphite strip.
const int silenceThreshold = 15;
const int minPitch = 150;
const int maxPitch = 2000;

void setup() {
  digitalWrite(trackPowerPin, LOW);
  pinMode(trackPowerPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Power the graphite track and allow it to settle.
  digitalWrite(trackPowerPin, HIGH);
  delayMicroseconds(1000);

  // Discard the first reading, then measure.
  analogRead(probePin);
  int reading = analogRead(probePin);

  // Switch off track power between measurements.
  digitalWrite(trackPowerPin, LOW);

  Serial.println(reading);

  if (reading <= silenceThreshold) {
    noTone(buzzerPin);
  } else {
    int pitch = map(
      reading,
      silenceThreshold + 1,
      1023,
      minPitch,
      maxPitch
    );

    tone(buzzerPin, pitch);
  }

  delay(10);
}