/*
Bipolar NEMA 17 stepper motor -> Change speed with potentiometer as input.
*/

const int POT_PIN = A0;

const int STEP = 2;
const int DIR = 3;

const int STEPS_PER_REV = 200; // 360°/1.8° = 200 pasos/revolución

void setup() {
  Serial.begin(9600);
  pinMode(POT_PIN, INPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  digitalWrite(DIR, HIGH);
}

void loop() {
  int pot_adc = analogRead(POT_PIN);
  int pulse_delay = map(pot_adc, 0, 1023, 100, 0);
  Serial.print("pot_ADC:");
  Serial.print(pot_adc);
  Serial.print(" | delay:");
  Serial.println(pulse_delay);
  delay(100);

  // if (pulse_delay < 100) {
  //   for (int i = 0; i < STEPS_PER_REV; i++) {
  //     digitalWrite(STEP, HIGH);
  //     delay(pulse_delay);
  //     digitalWrite(STEP, LOW);
  //     delay(pulse_delay);
  //   }
  // }
}
