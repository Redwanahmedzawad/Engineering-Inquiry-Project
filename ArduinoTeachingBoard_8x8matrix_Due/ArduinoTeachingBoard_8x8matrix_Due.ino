//Declare pins
int rows[8] = { 54, 55, 56, 57, 58, 59, 70, 71 };

int cols[8] = { 6, 7, 8, 9, 10, 11, 12, 13 };

void setup() {
  //Define pins
  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    pinMode(cols[i], OUTPUT);
  }

  for (int i = 0; i < 8; i++) {
    singleled(i, i);
    delay(100);
  }
}

void loop() {
  dotcycle();
  delay(100);
}

void dotcycle() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      ledreset();
      digitalWrite(rows[i], HIGH);
      digitalWrite(cols[j], LOW);
      delay(50);
    }
  }
}

void singleled(int x, int y) {
  ledreset();
  digitalWrite(rows[y], HIGH);
  digitalWrite(cols[x], LOW);
}

void ledreset() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(rows[i], LOW);
    digitalWrite(cols[i], HIGH);
  }
}