const int led = 8;
const int buttom = 2;

void setup() {
  pinMode(buttom, INPUT_PULLUP);
  pinMode(led, OUTPUT);
}

void loop() {
  int sensorVal = digitalRead(buttom);

  if (sensorVal == HIGH) {
    digitalWrite(led, LOW);
  } if (sensorVal == LOW) {
    digitalWrite(led, HIGH);
  }
}
