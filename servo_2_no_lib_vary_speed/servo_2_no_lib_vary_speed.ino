/*
  - Positional servo (no Servo.h lib) -> Control the rotational speed for a positional servo
  - Continous servo (no Servo.h lib) -> With the same code, see what happens with a continous servo
*/

const int PWM_SERVO = 8;
int pulse_width = 500;    // Starting pulse width for full speed clockwise
int increment = 10;       // Increment in microsecond for each step

void setup() {
  pinMode(PWM_SERVO, OUTPUT);
}

void loop() {
  // Send the PWM pulse
  digitalWrite(PWM_SERVO, HIGH);           // Start the pulse
  delayMicroseconds(pulse_width);          // Keep the pulse HIGH for the current pulse width
  digitalWrite(PWM_SERVO, LOW);            // End the pulse (set LOW)

  delayMicroseconds(20000 - pulse_width);  // Wait the ramainder of the 20ms

  // Adjust the pulse width
  pulse_width += increment;

  //Reverse direction when the boundaries are reached
  if (pulse_width >= 2500 || pulse_width <= 500){
    increment = -increment;
  }

  delay(20);  // Increase or decrease to control the speed of transition
}
