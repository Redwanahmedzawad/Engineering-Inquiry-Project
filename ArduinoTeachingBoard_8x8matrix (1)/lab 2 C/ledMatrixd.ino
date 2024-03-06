//Declare pins
#include"lib.h"
int rows[8] = { 14, 15, 16, 17, 18, 19, 20, 21 };
int cols[8] = { 6, 7, 8, 9, 10, 11, 12, 13 };
bool state=false;

void setup() {
  //Define pins
  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    pinMode(cols[i], OUTPUT);
  }
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  /*disp(1);
  //delay(20000000);
  clear();
  disp(0);*/
  if(!digitalRead(4))
  {
for(int j=0;j<16;j++){
 for(int i=0;i<10000;i++)
 {
   disp(j);
 }
  delay(1000);
}
clear();
}

}


void disp(int x)
{
 

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
void clear()
{
  for(int i=0;i<8;i++)
  {
    digitalWrite(rows[i],LOW);
    digitalWrite(cols[i],LOW);
  }
}

