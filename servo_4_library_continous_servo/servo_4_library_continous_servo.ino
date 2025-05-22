/*
  Controlling continous servo motors with the Servo.h library.
*/

/*
  Servo.h library functions:

    - attach(): links a servo to a specific pin on the Arduino board.
      servo.attach(pin);
      servo.attach(pin, min, max); min and max set the servo's range pulse width in microseconds.
      It helps to address calibration issues. Default min is 544, max is 2400.

    - write(): Sets the position of the servo by specifying an angle in degrees.
      If value is < 200 it's treated as an angle, otherwise as pulse width in microseconds.
      servo.write(angle);

    - writeMicroseconds(): Sets the position of the servo by specifying the pulse width in microseconds.
      servo.writeMicroseconds(pulse_width);

    - read() : Gets the last written servo pulse width as an angle between 0 and 180.
      int angle = servo.read();

    - readMicroseconds() : Gets the last written servo pulse width in microseconds. (was read_us() in first release).
      int pulse_width = servo.readMicroseconds();

    - attached(): Returns true if there is a servo attached.
      bool is_attached = servo.attached();

    - detach(): Stops an attached servo from pulsing its I/O pin.
      servo.detach();
*/

#include<Servo.h>

const int PWM_SERVO = 8;  // Define PWM servo pin from the Arduino board

Servo servo;  // Create a servo object

void setup() {
  // If the servos are well calibrated for the range (1.0, 1.5, 2.0) ms
  servo.attach(PWM_SERVO);

  // If the servo is not well calibrated, assign min and max pulse widths
  // servo.attach(PWM_SERVO, 500, 2500);
}

void loop() {
  // Pulse for full speed clockwise
  servo.writeMicroseconds(500);
  delay(2000);

  // Neutral pulse for stopping the servo
  servo.writeMicroseconds(1500);
  delay(2000);

  // Pulse for full speed counter-clockwise
  servo.writeMicroseconds(2500);
  delay(2000);
}
