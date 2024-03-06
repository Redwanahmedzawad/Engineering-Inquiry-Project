#include<time.h>
#include"lib.h"
#define ldrpin 5
const float GAMMA = 0.7;
const float RL10 = 50;
int rows[8] = { 14, 15, 16, 17, 18, 19, 20, 21 };
int cols[8] = { 6, 7, 8, 9, 10, 11, 12, 13 };
int i=-1;
void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
  
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  // long int s1= millis()/1000;
  // Serial.println(s1);
  for(int j=0;j<8;j++)
  {
    pinMode(rows[j],OUTPUT);
    pinMode(cols[j],OUTPUT);
  }
}

void loop() {  
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(A2);
  delay(100);
  int sval[3];
  int interval=10;
  if(analogValue>10)
  {
    i++;
    digitalWrite(6, HIGH);
    Serial.println(analogValue); 
    sval[i]=millis()/1000;
   
  }
  else
  {
    digitalWrite(6,LOW);
  }
  switch(i)
  {
    case 1:
      interval = sval[1]-sval[0];
      break;
    case 2:
      interval=sval[2]-sval[1];
     
      break;
  }
  if(i==2 && interval<3)
  {
   
      disp(0);
    
    Serial.println(i);
    i=-1;
    clear();
delay(500);
  }
  else if(i==2)
  {
    i=-1;
     disp(1);
    clear();
   
  }
  else{
    disp(1);
    clear();
    
  }
}

void disp(int x)
{
  for(int k=0;i<10000;k++){
      for(int i=0;i<8;i++)
    {
      digitalWrite(cols[i],LOW);
      for(int j=0;j<8;j++)
      {
        digitalWrite(rows[j], matrix[x][j][i]);
      }
      for(int j=0;j<8;j++)
      {
        digitalWrite(rows[j],LOW);
      }
      digitalWrite(cols[i],HIGH);
      
    }
  }
}
void clear()
{
  for(int i=0;i<8;i++)
  {
    digitalWrite(rows[i],LOW);
    digitalWrite(cols[i],LOW);
  }
}
