



#include<Stepper.h>

const int i1 = 8;
const int i2 = 9;
const int i3 = 10;
const int i4 = 11;

const int stepsPerRevolution = 2048;

// Definir la velocidad máxima
const int rpm = 10;

Stepper mi_motor(stepsPerRevolution, i1, i3, i2, i4);

void setup() {
  pinMode(i1, OUTPUT);
  pinMode(i2, OUTPUT);
  pinMode(i3, OUTPUT);
  pinMode(i4, OUTPUT);

  mi_motor.setSpeed(rpm);
}

void loop() {
  mi_motor.step(stepsPerRevolution);
  delay(1000);

  mi_motor.step(-stepsPerRevolution);
  delay(1000);
}
