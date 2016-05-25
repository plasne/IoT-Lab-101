int flash = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
}

void loop() {

  delay(1000);
  if (flash == HIGH) {
    Serial.println("on");
    flash = LOW;
  } else {
    Serial.println("off");
    flash = HIGH;
  }
  digitalWrite(5, flash);

}

