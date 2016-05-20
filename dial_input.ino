void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop() {

  int sensorValue = analogRead(A0);
  Serial.println("value: ");
  Serial.println(sensorValue);
  delay(200);

  if (sensorValue > 800) {
    digitalWrite(4, HIGH);
    digitalWrite(13, LOW);
  } else if (sensorValue < 200) {
    digitalWrite(4, LOW);
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(4, LOW);
    digitalWrite(13, LOW);
  }

}
