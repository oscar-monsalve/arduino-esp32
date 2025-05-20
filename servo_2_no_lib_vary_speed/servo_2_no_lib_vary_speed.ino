/*
  - Positional servo (no Servo.h lib) -> Set the desired pulse width and maintain position
  - Continous servo (no Servo.h lib) -> With the same code, see what happens with a continous servo
*/

const int PWM_SERVO = 8;

void setup() {
  pinMode(PWM_SERVO, OUTPUT);
}

/**
 * Send PWM pulses to set and hold the servo position.
 *@param pulse_width Desired pulse width in microseconds.
 *@param hold_time Desired hold time in miliseconds to stop the servo.
*/
void hold_position(int pulse_width, int hold_time) {
  unsigned long start_time = millis();  // Initialize the start time
  while (millis() - start_time < hold_time) {
    digitalWrite(PWM_SERVO, HIGH);            // Start the PWM pulse
    delayMicroseconds(pulse_width);           // Keep it HIGH for the specified pulse width
    digitalWrite(PWM_SERVO, LOW);             // End the pulse setting it to LOW
    delayMicroseconds(20000 - pulse_width);   // Wait for the remainder of the 20 ms PWM signal period
  }
}

void loop() {

}
