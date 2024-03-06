#include "lib.h"
#include <DFRobot_BMX160.h>

DFRobot_BMX160 bmx160;
int rows[8] = { 14, 15, 16, 17, 18, 19, 20, 21 };
int cols[8] = { 6, 7, 8, 9, 10, 11, 12, 13 };
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
    for(int i=0;i<8;i++)
  {
    pinMode(rows[i],OUTPUT);
    pinMode(cols[i],OUTPUT);
  }
  if(bmx160.begin()!=true)
  {
    Serial.println("init false");
    while(1);
  }

}

void loop() {
  sBmx160SensorData_t Omagn, Ogyro, Oaccel;
  // put your main code here, to run repeatedly:
   bmx160.getAllData(&Omagn, &Ogyro, &Oaccel);
   float x = Oaccel.x;
  if(Oaccel.x>0 &&Oaccel.x>Oaccel.y)
  {
    Serial.print("X");
    Serial.println(Oaccel.x);
    clear();
    disp(0);
    
  }
 else if(Oaccel.x<0 && Oaccel.x<Oaccel.y)
  {
    Serial.print("Y");
     Serial.println(Oaccel.y);
    clear();
    disp(3);
    delay(500);
    
  }
 else if (Oaccel.y>0 && Oaccel.y>Oaccel.x)
{ Serial.print("Y");
     Serial.println(Oaccel.y);
    clear();
    disp(2);
    delay(500);
    }
    else if (Oaccel.y<0 && Oaccel.y<Oaccel.x)
{
Serial.print("Y");
     Serial.println(Oaccel.y);
    clear();
    disp(1);
    delay(500);
}
}
void disp(int x)
{
  for(int k=0;k<2000;k++){
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

