#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#include <EEPROM.h>
#define TFT_CS    10
#define TFT_RST   6 
#define TFT_DC    7 

#define TFT_SCLK 13   
#define TFT_MOSI 11  

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
void setup() {
  // put your setup code here, to run once:
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.setCursor(2,2);
  tft.fillScreen(ST77XX_BLACK); 
  byte id[] = "103501849";

  EEPROM.write(10, 103501849);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  byte id_r[9];
  for(int i=0;i<9;i++)
  {
    id_r[i]=EEPROM.read[i];
  }
  tft.println("Student ID;");
  tft.setCursor(0,15);
  tft.write(id_r[],9);

  delay(1000);

}
