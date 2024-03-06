#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include "RTClib.h"
RTC_DS1307 rtc;
#define TFT_CS    10
#define TFT_RST   6 
#define TFT_DC    7 

#define TFT_SCLK 13   
#define TFT_MOSI 11   

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rtc.begin();
 rtc.adjust(DateTime(2023,9,21,10,8,0));
    tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.setCursor(2,2);
  tft.fillScreen(ST77XX_BLACK);  
  //tft.println("Hello World");
}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now =rtc.now();
  tft.fillScreen(ST77XX_BLACK); 
  tft.setCursor(2,2);
  tft.print(now.hour(),DEC);
  tft.print(":");
  tft.print(now.minute(),DEC);
  tft.print(":");
  tft.print(now.second(),DEC);
  
  delay(1000);
}
