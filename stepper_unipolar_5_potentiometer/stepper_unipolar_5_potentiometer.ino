/*
  Stepper unipolar motor -> Controls the motor speed using a potentiometer as input.
*/

#include <Stepper.h>

// Stepper motor pins and data
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;
const int STEPS_PER_REV = 2048;
const int RPM = 10;

// Potentiometer input pin
const int POT_PIN = A0;

// State variables
int pot_input_adc;
int set_speed;

Stepper stepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(9600);
  pinMode(POT_PIN, INPUT);
}

void loop() {
  pot_input_adc = analogRead(POT_PIN);
  set_speed = map(pot_input_adc, 0, 1023, 0, 15);
  // Serial.print("pot_ADC:");
  // Serial.print(pot_input_adc);
  // Serial.print(" | speed:");
  // Serial.println(set_speed);
  // delay(100);

  if (set_speed > 0) {
    stepper.setSpeed(set_speed);
    stepper.step(1);  // steps 1/100 of a revolution
  }
}
