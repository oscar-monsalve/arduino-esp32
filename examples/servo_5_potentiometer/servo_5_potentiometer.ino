/*
  Controlling positional and continous servo motors with a potentiometer and the Servo.h lib
*/

#include<Servo.h>

const int POT_PIN = A0;   // Analog input pin for the potentiometer
const int PWM_SERVO = 8;  // PWM servo output pin from the Arduino board
int pot_adc_value;  // Declare variable to store the ADC potentiometer value
int servo_angle;    // Declare variable to store the servo angle

Servo servo;  // Create a servo object

void setup() {
  Serial.begin(9600);
  servo.attach(PWM_SERVO);
  pinMode(POT_PIN, INPUT);
  pinMode(PWM_SERVO, OUTPUT);
}

void loop() {
  pot_adc_value = analogRead(POT_PIN);
  servo_angle = map(pot_adc_value, 0, 1023, 0, 180);
  servo.write(servo_angle);

  Serial.print("POT_ADC: ");
  Serial.print(pot_adc_value);
  Serial.print(" | Angle: ");
  Serial.println(servo_angle);
  delay(15);
}
