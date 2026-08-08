/*
  Controlling positional and continous servo motors with a potentiometer and the Servo.h lib

  Joystick ADC values:

             0
             |
             |
             |(521, 511)
  0 ---------●----------->1023
             |         VRX
             |
             |
             |
             ﹀VRY
             1023

*/

#include<Servo.h>

const int JOYSTICK_PIN_X = A0;   // Analog input pin for the joystick x direction
const int JOYSTICK_PIN_Y = A1;   // Analog input pin for the joystick y direction
const int PWM_SERVO_X = 8;  // PWM servo output pin from the Arduino board
const int PWM_SERVO_Y = 9;  // PWM servo output pin from the Arduino board

int joystick_adc_x;  // Declare variable to store the ADC joystick x direction
int joystick_adc_y;  // Declare variable to store the ADC Joystick y  direction
int servo_angle_x;   // Declare variable to store the servo angle x direction
int servo_angle_y;   // Declare variable to store the servo angle y direction

Servo servo_x;  // Create a servo object
Servo servo_y;  // Create a servo object

void setup() {
  Serial.begin(9600);
  servo_x.attach(PWM_SERVO_X);
  servo_y.attach(PWM_SERVO_Y);
  pinMode(JOYSTICK_PIN_X, INPUT);
  pinMode(JOYSTICK_PIN_Y, INPUT);
  pinMode(PWM_SERVO_X, OUTPUT);
  pinMode(PWM_SERVO_Y, OUTPUT);
}

void loop() {
  joystick_adc_x = analogRead(JOYSTICK_PIN_X);
  joystick_adc_y = analogRead(JOYSTICK_PIN_Y);
  servo_angle_x = map(joystick_adc_x, 0, 1023, 0, 180);
  servo_angle_y = map(joystick_adc_y, 0, 1023, 0, 180);
  servo_x.write(servo_angle_x);
  servo_y.write(servo_angle_y);

  // Print serial monitor for debbugging
  Serial.print("(adc:x, y): ");
  Serial.print(joystick_adc_x);
  Serial.print(" ");
  Serial.print(joystick_adc_y);

  Serial.print(" | (angle:x, y): ");
  Serial.print(servo_angle_x);
  Serial.print(" ");
  Serial.println(servo_angle_y);
  delay(10);
}
