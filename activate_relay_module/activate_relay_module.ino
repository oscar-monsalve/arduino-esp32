// This program waits for a button to be pushed to activate a relay for 10 seconds.

const int INPUT_BUTTON = 22;
const int OUTPUT_RELAY = 26;

void setup() {
  pinMode(INPUT_BUTTON, INPUT);
  pinMode(OUTPUT_RELAY, OUTPUT);
}

void loop() {
  int status = digitalRead(INPUT_BUTTON);

  if (status == HIGH) {
    digitalWrite(OUTPUT_RELAY, HIGH);
    delay(10000);
  }
  else {
    digitalWrite(OUTPUT_RELAY, LOW);
  }

}
