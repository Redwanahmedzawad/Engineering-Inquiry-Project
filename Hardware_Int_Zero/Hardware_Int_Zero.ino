//Example - Exernal interrupt
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#define TFT_CS    10
#define TFT_RST   6 
#define TFT_DC    7 

#define TFT_SCLK 13   
#define TFT_MOSI 11  
#define LED 11
#define BUTTON_PIN 2
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // setup serial
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(BUTTON_PIN,displaymic,LOW);
  delay(500); //start the int.0   // opton 1 to configure hardware/external interupt
 // attachInterrupt(digitalPinToInterrupt(21), displaymic,RISING); // option 2 

}

//ISR - external interupt
void displaymic(){
  tft.print("@");
  delay(10000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // turn on/off LED
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);

}
