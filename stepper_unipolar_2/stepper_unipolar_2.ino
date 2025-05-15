/*
  Stepper unipolar motor -> Asks the user to input an angle to move the moto to. Then, it returns back to the initial position
*/

#include<Stepper.h>

// Digital output PWM pins
const int i1 = 8;
const int i2 = 9;
const int i3 = 10;
const int i4 = 11;

const int stepsPerRevolution = 2048;
const int rpm = 7;

float currentAngle = 0.0;  // Asume that the motor starts at 0°. Can't be a const because it needs to be reassigned in the loop

Stepper myStepper(stepsPerRevolution, i1, i3, i2, i4);

void setup() {
  pinMode(i1, OUTPUT);
  pinMode(i2, OUTPUT);
  pinMode(i3, OUTPUT);
  pinMode(i4, OUTPUT);
  myStepper.setSpeed(rpm);

  Serial.begin(9600);
  Serial.println("Enter an angle between 0° and 360°:");
}

void loop() {
  if (Serial.available()) {
    float targetAngle = Serial.parseFloat();

    if (targetAngle >= 0 && targetAngle <= 360) {
      float deltaAngle = targetAngle - currentAngle;
      int stepsToMove = (int)((deltaAngle / 360) * stepsPerRevolution);

      myStepper.step(stepsToMove);
      currentAngle = targetAngle;

      Serial.print("Moved to angle: ");
      Serial.println(currentAngle);
      delay(500);
    } else {
      Serial.println("Invalid angle. Enter a value between 0 and 360.");
    }
    delay(500);
    Serial.println("Enter next angle to move to:");
  }

}
