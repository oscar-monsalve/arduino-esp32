/*
Bipolar NEMA 17 stepper motor -> Change direction and rotational speed with for loops.
*/

const int step = 2;
const int dir = 3;

const int stepsPerRevolution = 200; // 360°/1.8° = 200 pasos/revolución
const int pulseDelayFast = 1; // change the speed of the motor in miliseconds
const int pulseDelaySlow = 10; // change the speed of the motor in miliseconds

void setup() {
  pinMode(step, OUTPUT);
  pinMode(dir, OUTPUT);

  delay(2000);
}

void loop() {
  digitalWrite(dir, HIGH);  // cambiar dirección de giro
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(step, HIGH);
    delay(pulseDelayFast);
    digitalWrite(step, LOW);
    delay(pulseDelayFast);
  }
  delay(500);

  digitalWrite(dir, LOW);  // cambiar dirección de giro
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(step, HIGH);
    delay(pulseDelaySlow);
    digitalWrite(step, LOW);
    delay(pulseDelaySlow);
  }
  delay(500);
}
