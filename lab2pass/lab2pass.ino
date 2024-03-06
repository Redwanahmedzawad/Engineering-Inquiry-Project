bool buttonstate = 0;
bool ledd10=0;
bool ledon= 0;
void setup() {
  Serial.begin(9600); // start serial communication at 9600 baud
  for (int i = 2; i != 6; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  for(int i=8;i<12;i++)
  {
    pinMode(i,OUTPUT);
  }
}

void loop() {
  for (int j = 2; j < 6; j++) {
    buttonstate = digitalRead(j);
    if (!buttonstate) {
      //Serial.print("Button ");
      if((j-1)==1)
      {
        if(ledon==0){
          digitalWrite(11,HIGH);
          delay(500);
          ledon=1;
        }
        else
        {
          digitalWrite(11,LOW);
          delay(500);
          ledon = 0;
        }
      }
    }
  }
}