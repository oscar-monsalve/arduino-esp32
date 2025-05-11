const int INPUT_BUTTON = 22
const int OUTPUT_LED = 21

void setup() {
  Serial.begin(115200);
  pinMode(INPUT_BUTTON, INPUT);
  pinMode(OUTPUT_LED, OUTPUT);
}

void loop() {
  int a = digitalRead(INPUT_BUTTON);
  if(a == 1) {
    digitalWrite(OUTPUT_LED, HIGH);
    Serial.println("PRESSED");
  }
  if(a == 0) {
    digitalWrite(OUTPUT_LED, LOW);
    Serial.println("NOT PRESSED");
  }
  delay(500);
}
