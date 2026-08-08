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

const int JOYSTICK_PIN_X = A0;   // Analog input pin for the joystick
const int PWM_SERVO = 8;  // PWM servo output pin from the Arduino board
int joystick_adc_value;  // Declare variable to store the ADC potentiometer value
int servo_angle;    // Declare variable to store the servo angle

Servo servo;  // Create a servo object

void setup() {
  Serial.begin(9600);
  servo.attach(PWM_SERVO);
  pinMode(JOYSTICK_PIN_X, INPUT);
  pinMode(PWM_SERVO, OUTPUT);
}

void loop() {
  joystick_adc_value = analogRead(JOYSTICK_PIN_X);
  servo_angle = map(joystick_adc_value, 0, 1023, 0, 180);
  servo.write(servo_angle);

  Serial.print("JOYSTICK_X_ADC: ");
  Serial.print(joystick_adc_value);
  Serial.print(" | Angle: ");
  Serial.println(servo_angle);
  delay(100);
}
