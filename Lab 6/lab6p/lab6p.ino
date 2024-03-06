//Example - Exernal interrupt
 
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#define TFT_CS    10
#define TFT_RST   6 
#define TFT_DC    7 

#define TFT_SCLK 13   
#define TFT_MOSI 11  
#include <avr/pgmspace.h>
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST); 
int rows[8] = { 14, 15, 16, 17, 18, 19, 20, 21 };
int cols[8] = { 6, 7, 8, 9, 10, 11, 12, 13 };
int x=0;
void setup() {
  // put your setup code here, to run once:
  tft.initR(INITR_BLACKTAB); 
 // tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
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
  tft.setCursor(2,2);
  tft.setTextSize(5);
  tft.print("@");
  delay(100);
}


void loop() {
  // put your main code here, to run repeatedly:
  // turn on/off LED
  

}
