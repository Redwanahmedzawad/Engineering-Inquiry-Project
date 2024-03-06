#define ldrpin 5
const float GAMMA = 0.7;
const float RL10 = 50;

void setup() {
  // put your setup code here, to run once:
  pinMode(5, INPUT_PULLUP);
  Serial.begin(9600);
  
  pinMode(6,OUTPUT);

}

void loop() {  
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(A2);
  analogWrite(6,analogValue);
  
  
}

