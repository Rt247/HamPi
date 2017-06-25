int pulse() {
  digitalWrite(CLOCK, HIGH);
  delayMicroseconds(1);
  int res = digitalRead(DATA);
  digitalWrite(CLOCK, LOW);
  delayMicroseconds(1);
  return res;
}

void nesInitialise(){
  wiringPiSetupGpio();
  // DATA
  pinMode(DATA, INPUT);
  // LATCH
  pinMode(LATCH, OUTPUT);
  // CLOCK
  pinMode(CLOCK, OUTPUT);
}

void inputStage(){
  digitalWrite(LATCH, LOW);
  digitalWrite(CLOCK, LOW);

  // Latch and delay
  digitalWrite(LATCH, HIGH);
  delayMicroseconds(1);
  digitalWrite(LATCH, LOW);

  nesController.stateA = digitalRead(DATA);
  nesController.stateB = pulse();
  nesController.stateSelect = pulse();
  nesController.stateStart = pulse();
  nesController.stateUp = pulse();
  nesController.stateDown = pulse();
  nesController.stateLeft = pulse();
  nesController.stateRight = pulse();
}
