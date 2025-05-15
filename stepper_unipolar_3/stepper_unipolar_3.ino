/*
  Stepper unipolar motor -> Asks the user to input an angle to move the motor, and the motor stays in that position.
  It uses the AccelStepper.h library.
*/

#include<AccelStepper.h>

const int i1 = 8;
const int i2 = 9;
const int i3 = 10;
const int i4 = 11;

const int stepsPerRevolution = 2048;
const int rpm = 7;

float currentAngle = 0.0;  // Asume that the motor starts at 0°. Can't be a const because it needs to be reassigned in the loop

AccelStepper myStepper(AccelStepper::FULL4WIRE, i1, i3, i2, i4);

void setup() {
  Serial.begin(9600);
  myStepper.setMaxSpeed(500);
  myStepper.setAcceleration(250);
  Serial.println("Enter angle between 0° and 360°:");
}

void loop() {
  if (Serial.available()) {
    float targetAngle = Serial.parseFloat();

    if (targetAngle >= 0 && targetAngle <= 360) {
      float deltaAngle = targetAngle - currentAngle;
      long stepsToMove = (long)((deltaAngle / 360.0) * stepsPerRevolution);

      myStepper.move(stepsToMove);

      while (myStepper.distanceToGo() != 0) {
        myStepper.run();
      }
      // Keep coils energized to hold position
      myStepper.enableOutputs();

      // currentAngle = targetAngle;  // this line returns the motor to its original position

      Serial.print("Moved to angle: ");
      Serial.println(currentAngle);
    } else {
      Serial.println("Invalid input. Enter angle between 0 and 360.");
    }

    Serial.println("Enter next angle:");
    delay(1000);
  }
}
