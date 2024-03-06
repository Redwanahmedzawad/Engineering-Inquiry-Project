bool buttonstate = 0;
bool ledd10=0;
bool ledon= 0;
int tim;
void setup() {
  Serial.begin(9600); // start serial communication at 9600 baud
  for (int i = 2; i != 6; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  for(int i=6;i<12;i++)
  {
    pinMode(i,OUTPUT);
  }
  tim = 1000;
}

void loop() {
  digitalWrite(10,HIGH);
  delay(tim);
  digitalWrite(10,LOW);
  delay(tim);
  if(!digitalRead(3))
  {
      tim=100;
      
  }
  if(!digitalRead(4))
  {
    tim=1000;
  }
  if(!digitalRead(5))
  {
    for(int i=0;i<256;i++)
    {
      analogWrite(9,i);
      analogWrite(6,i);
      delay(10);
    }
    for(int i=256;i>0;i--)
    {
      analogWrite(9,i);
      analogWrite(6,i);
      delay(10);
    }
  }
}