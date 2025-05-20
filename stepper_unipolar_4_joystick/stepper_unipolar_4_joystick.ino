#include <Stepper.h>

// Stepper motor pins and data
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;
const int STEPS_PER_REV = 2048;
const int RPM = 10;

// Joystick pins
const int X_PIN = A0;

// State variables
int x_input;
int y_input;

Stepper stepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(9600);

  pinMode(X_PIN, INPUT);
  stepper.setSpeed(RPM);
}

void loop() {
  x_input = analogRead(X_PIN);
  // Serial.print("x:");
  // Serial.println(x_input);

  if (x_input > 400 && x_input < 600) {
    for (int i = 2; i < 6; i++) {
      digitalWrite(i, LOW);
    }
  } else if (x_input < 400) {
    stepper.setSpeed(RPM);
    stepper.step(-30);
  } else if (x_input > 600) {
    stepper.setSpeed(RPM);
    stepper.step(30);
  }
}
