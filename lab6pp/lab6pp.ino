//Example - Exernal interrupt
#include "lib.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
int rows[8] = { 14, 15, 16, 17, 18, 19, 20, 21 };
int cols[8] = { 6, 7, 8, 9, 10, 11, 12, 13 };
int x=0;
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // setup serial
    for(int j=0;j<8;j++)
  {
    pinMode(rows[j],OUTPUT);
    pinMode(cols[j],OUTPUT);
  }

  pinMode(2,INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(i), displaymic,RISING);
  
  attachInterrupt(2,displaymic,LOW);
  
 // delay(500); //start the int.0   // opton 1 to configure hardware/external interupt
 // attachInterrupt(digitalPinToInterrupt(21), displaymic,RISING); // option 2 

}

//ISR - external interupt
void displaymic(){
  
  for(int k=0;k<10000;k++){
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
  x++;
  clear();
}
void clear()
{
  for(int i=0;i<8;i++)
  {
    digitalWrite(rows[i],LOW);
    digitalWrite(cols[i],LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // turn on/off LED
  

}
