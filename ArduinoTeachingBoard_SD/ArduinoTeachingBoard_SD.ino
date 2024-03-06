#include "SdFat.h"
#include "RTClib.h"
RTC_DS1307 rtc;
//define file system for sd card
SdFs sd;
FsFile file;
//^ configuration for FAT16/FAT32 and exFAT.

// Chip select may be constant or RAM variable.
const uint8_t SD_CS_PIN = A3;
//
// Pin numbers in templates must be constants.
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN  = 13;

// SdFat software SPI template
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(0), &softSpi)
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
  Serial.begin(9600);
   rtc.begin();
 rtc.adjust(DateTime(2023,9,21,10,8,0));
  if (!sd.begin(SD_CONFIG)) {
    Serial.println("SD card initialization failed!");
    sd.initErrorHalt();

    while (1);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.setCursor(2,2);
  tft.fillScreen(ST77XX_BLACK); 
  }

  // Open/create a file for writing
  if (!file.open("message.txt", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

  file.close(); //release file

  Serial.println("Please enter a message and press enter:");
}

void loop() {
  DateTime now =rtc.now();
  Serial.print(now.hour(),DEC);
  Serial.print(":");
  Serial.print(now.minute(),DEC);
  Serial.print(":");
  Serial.print(now.second(),DEC);
  String msg = String(now.hour())+":"+String(now.minute())+":"+String(now.second());

    
   // WriteSD(file, msg);
    
    ReadSD(file);
}

void WriteSD(File file, String message) {  
  Serial.println("---   Saving To File   ---");

  file.open("message.txt", O_RDWR);
  file.rewind();                //Go to file position 0
  file.println("Message written using SoftwareSPI");
  file.println(message);
  file.close();
}

void ReadSD(File file) {
  Serial.println("--- Reading From file! ---");
  
  file.open("message.txt", O_RDWR);
  file.seek(0);                 //go to char 0
  String contents = file.readString();
  Serial.println(contents[9]);
    tft.fillScreen(ST77XX_BLACK); 
    tft.println(contents);
  file.close();
}