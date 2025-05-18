/*
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

const int x_pin = A0;
const int y_pin = A1;
const int button_pin = 8;

int x_input;
int y_input;
int button_state;

void setup() {
  Serial.begin(9600);

  pinMode(x_pin, INPUT);
  pinMode(y_pin, INPUT);
  pinMode(button_pin, INPUT_PULLUP);
}

void loop() {
  x_input = analogRead(x_pin);
  y_input = analogRead(y_pin);
  button_state = digitalRead(button_pin);

  Serial.print("x:");
  Serial.print(x_input);
  Serial.print(" | y:");
  Serial.print(y_input);
  Serial.print(" | button:");
  Serial.println(button_state);
  delay(100);
}
