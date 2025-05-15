/*
 Unipolar stepper motor -> Alternating continuos rotation in both directions (Considered as a "Hello World" program)
*/

#include <Stepper.h>

const int i1 = 8;
const int i2 = 9;
const int i3 = 10;
const int i4 = 11;

// step angle 5.625/64 and 1/32 gear ratio -> stepsPerRevolution = 64 * 32
const int stepsPerRevolution = 64 * 32;

// max_rpm = (steps_per_second / stepsPerRevolution) * 60 -> max_rpm = (500 pps/(64*32))*60 = 14.6 rpm aprox.= 15 rpm
const int rpm = 10;

// Tener que las salidas i3 e i2 cambian de posición en la función Stepper::myStepper
Stepper myStepper(stepsPerRevolution, i1, i3, i2, i4);

void setup() {
  pinMode(i1, OUTPUT);
  pinMode(i2, OUTPUT);
  pinMode(i3, OUTPUT);
  pinMode(i4, OUTPUT);
  myStepper.setSpeed(rpm);
}

void loop() {
  myStepper.step(stepsPerRevolution); // 1 full rotation in one direction
  delay(1000);

  myStepper.step(-stepsPerRevolution); // 1 full rotation in the other direction
  delay(1000);
}
