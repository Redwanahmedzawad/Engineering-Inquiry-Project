
int opnum;
int ledon=0;
int tim = 200;
bool once = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int opt[4]={1,2,3,4};
  Serial.println("Options: ");
  Serial.println("1. Toggle");
  Serial.println("2. Increase blinking speed");
  Serial.println("3. Decrease blinking Speed");
  Serial.println("4. Buzzer PWM");
 
  pinMode(11, OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(6,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(10,HIGH);
  delay(tim);
  digitalWrite(10,LOW);
  delay(tim);
  if(Serial.available()){
    char ch=Serial.read();
    if(isDigit(ch))
    {
        opnum = (ch-'0');
    }
  }
  if(opnum == 1)
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
        opnum=9;
        
  }
  switch(opnum){
    case 2:
      tim = 100;
      break;
    case 3:
      tim=500;
      break;
    case 4:
      for(int i=0;i<256;i++)
      {
        analogWrite(6,i);
        analogWrite(9,i);
        delay(100);
      }
  }
 
}
