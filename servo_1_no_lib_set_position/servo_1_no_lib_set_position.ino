/*
  - Positional servo (no Servo.h lib) -> Set the desired pulse width and maintain position
  - Continous servo (no Servo.h lib) -> With the same code, see what happens with a continous servo
*/

const int PWM_SERVO_1 = 8;
const int PWM_SERVO_2 = 9;  // add another servo

void setup() {
  pinMode(PWM_SERVO_1, OUTPUT);
  pinMode(PWM_SERVO_2, OUTPUT);
}

/**
 * Sends PWM pulses to set and hold the servo position.
 *@param pulse_width: Desired pulse width in microseconds.
 *@param hold_time: Desired hold time in miliseconds to stop the servo.
*/
void hold_position(int pulse_width, int hold_time) {
  unsigned long start_time = millis();  // Initialize the start time
  while (millis() - start_time < hold_time) {
    digitalWrite(PWM_SERVO_1, HIGH);            // Start the PWM pulse
    digitalWrite(PWM_SERVO_2, HIGH);            // Start the PWM pulse for another servo
    delayMicroseconds(pulse_width);           // Keep it HIGH for the specified pulse width
    digitalWrite(PWM_SERVO_1, LOW);             // End the pulse setting it to LOW
    digitalWrite(PWM_SERVO_2, LOW);            // Start the PWM pulse for another servo
    delayMicroseconds(20000 - pulse_width);   // Wait for the remainder of the 20 ms PWM signal period
  }
}

void loop() {
  // Note: some servos are not well enough calibrated to operate with exact pulse widths of 1ms and 2ms
  // It might be neccesary to lower and increase it to 0.5ms and 2.5ms, respectively

  // Positional servo: Move servo to 0 degrees (pulse width of +0.5 ms) and hold for 2 seconds
  // Continuos servo: Move servo at full speed clock-wise for 2 seconds (pulse width of +1.0 ms)
  hold_position(500, 2000);

  // Positional servo: Move servo to 90 degrees (pulse width of +1.5 ms) and hold for 2 seconds
  // Continuos servo: Stop servo for 2 seconds (pulse width of +1.0 ms)
  hold_position(1500, 2000);

  // Positional servo: Move servo to 180 degrees (pulse width of +2.5 ms) and hold for 2 seconds
  // Continuos servo: Move servo at full speed counter clock-wise for 2 seconds (pulse width of +2.5 ms)
  hold_position(2500, 2000);
}
