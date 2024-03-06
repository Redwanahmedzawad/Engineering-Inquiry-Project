#define ldrpin 2
const float GAMMA = 0.7;
const float RL10 = 50;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
  for(int i=3;i<12;i++)
  {
  pinMode(i,OUTPUT);
  }
}

void loop() {  
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(A0);
  for(int i=3;i<12;i++)
  {
    analogWrite(i,analogValue);
    //delay(50);
  }
}

