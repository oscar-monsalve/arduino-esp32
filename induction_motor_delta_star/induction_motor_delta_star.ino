// Program to control the sequence of delta-star induction motor start

// Inputs:
// - Stop button
// - Right turn button
// - Left turn button

// Outputs:
// - KM1 (left turn)
// - KM2 (right turn)
// - KM3 (delta conection)
// - KM4 (star conection)

// Inputs
const int STOP = 19;
const int RIGHT_START = 23;
const int LEFT_START = 18;

// Outputs
const int KM1 = 32;
const int KM2 = 33;
const int KM3 = 34;
const int KM4 = 35;

// Button states
int stop_state = 0;
int right_state = 0;
int left_state = 0;

bool stopped_system = true;  // checks if the system is stopped to avoid pressing right and left at the same time

void setup() {
  pinMode(STOP, INPUT);
  pinMode(RIGHT_START, INPUT);
  pinMode(LEFT_START, INPUT);

  pinMode(KM1, OUTPUT);
  pinMode(KM2, OUTPUT);
  pinMode(KM3, OUTPUT);
  pinMode(KM4, OUTPUT);
}

void loop() {


}
