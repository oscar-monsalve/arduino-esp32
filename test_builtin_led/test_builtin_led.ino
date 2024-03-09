#define LED_BUILTIN 5 //for ESPWROOM lolin32

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.println("Hello");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);

  Serial.println("World!");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
