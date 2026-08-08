/*
Bipolar NEMA 17 stepper motor -> Microstepping

| MS1  | MS2  | MS3  | RESOLUCIÓN    |
| ---- | ---- | ---- | ------------- |
| LOW  | LOW  | LOW  | Paso completo |
| HIGH | LOW  | LOW  | 1/2 paso      |
| LOW  | HIGH | LOW  | 1/4 paso      |
| HIGH | HIGH | LOW  | 1/8 paso      |
| HIGH | HIGH | HIGH | 1/16 paso     |

The higher the resolution is, the low the torque of the motor.
*/

const int step = 2;
const int dir = 3;

// Pins for microstepping
const int MS1 = 8;
const int MS2 = 9;
const int MS3 = 10;

const int stepsPerRevolution = 200; // 360°/1.8° = 200 pasos/revolución
const int pulseDelay= 10; // change the speed of the motor in miliseconds

void setup() {
  pinMode(step, OUTPUT);
  pinMode(dir, OUTPUT);

  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  delay(2000);  // esperar a que comience el loop()

  digitalWrite(dir, HIGH);  // cambiar dirección de giro

  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
}

void loop() {
  digitalWrite(step, HIGH);
  delay(250);

  digitalWrite(step, LOW);
  delay(250);
}
