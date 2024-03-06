void setup() {
  Serial.begin(9600);  // start serial communication at 9600 baud
  for (int i = 2; i != 6; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  for (int j = 2; j != 6; j++) {
    int switchState = digitalRead(j);
    if (switchState) {
      Serial.print("Switch ");
      Serial.println(j + 4);
    }
    digitalWrite(j + 7, switchState);
  }
}
