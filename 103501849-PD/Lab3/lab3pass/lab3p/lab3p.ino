#define ldrpin 2
const float GAMMA = 0.7;
const float RL10 = 50;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
  for(int i=3;i<11;i++)
  {
    pinMode(i,OUTPUT);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(A1);


  if(analogValue>15)
  {
     Serial.println(analogValue);
    for(int i=3;i<11;i++)
  {
    digitalWrite(i,LOW);
  }
  }
  else{

  disp();
    Serial.println(analogValue);
    delay(500);
    }
  
  

}
void disp()
{
  for(int i=3;i<11;i++)
  {
    digitalWrite(i,HIGH);
  }
}
