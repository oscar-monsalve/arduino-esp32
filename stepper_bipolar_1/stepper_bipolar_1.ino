/*
Bipolar NEMA 17 stepper motor "Hello World" -> Continous motion
  */

const int step = 2;
const int dir = 3;

const int stepsPerRevolution = 200; // 360°/1.8° = 200 pasos/revolución

void setup() {
  pinMode(step, OUTPUT);
  pinMode(dir, OUTPUT);

  delay(2000);

  digitalWrite(dir, HIGH);  // cambiar dirección de giro
}

void loop() {
  digitalWrite(step, HIGH);
  delayMicroseconds(5000);

  digitalWrite(step, LOW);
  delayMicroseconds(5000);
}
