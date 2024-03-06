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

#include "RTClib.h"
RTC_DS1307 rtc;

#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#include <BH1750.h>
BH1750 lightMeter(0x23); 
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C

  // Initialise menu logic
int Select = 1; // for incoming serial data
int Confirm = 0; // for incoming serial data
int Cancel = 0; // for incoming serial data
static int BME680menu = 0;
static int BH1750menu = 0;
static int Main_Menu= 0;


void setup() {

  Serial.println("Hello World!");
  Serial.begin(9600);  // start serial communication at 9600 baud

  // Initialise buttons
  pinMode(2,INPUT_PULLUP); // Button 1
  pinMode(3,INPUT_PULLUP); // Button 2
  pinMode(4,INPUT_PULLUP); // Button 3
  pinMode(5,INPUT_PULLUP); // Button 4
  attachInterrupt(2,Menu1,RISING);
  //Commented out below as these interrupts cause Arduino Due to not work
  //attachInterrupt(3,Menu2,RISING);
  //attachInterrupt(4,Menu3,RISING); 
  attachInterrupt(5,Menu4,RISING); 


  // Initialise TFT
  tft.initR(INITR_BLACKTAB);   
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);  // White on black
  tft.setTextWrap(true);  // Don't wrap text to next line
  tft.setTextSize(1);  // large letters
  tft.setRotation(3); // horizontal display

}
void loop() {
  int Select_2 = 1;
  Confirm =0; //
Cancel =0;

  Serial.println(Select); // Show current value of Select in Serial Monitor

// Wait until a selection is confirmed.
while (Confirm == 0){
 // Print out menu only once
  if (Main_Menu == 0){
    tft.setCursor(1, 1);
    tft.fillScreen(ST77XX_BLUE);
    tft.println("  Main Menu");
    tft.println(" 1 : BME680 Sensor");
    tft.println(" 2 : BH1750FVI Sensor");
    tft.println();
    tft.println(" Please input your Option 1-2");
    Main_Menu = 1; // Main menu will only show once
  }
 // Check for change in selection
  if (Serial.available() > 0) {
    // wait for a user input
    Select_2 = Serial.read() - '0';
    if (Select_2 >0 && Select_2 <10)
    Select= Select_2;
  }
  // Display selection value in TFT
  Serial.println(Select);
  tft.setCursor(20, 100);
  tft.print("Current Selection:");
  tft.setCursor(100, 110);
  tft.print(Select);
  delay(1000);
}
Cancel = 0; // important to reset Cancel back to 0 so we don't exit twice accidentally
// Option 1 (Atmosphere Sensor Menu)
if (Select == 1)
{  
  // Stay in BME680 Sensor menu until exit/cancel is chosen
  while (Cancel == 0){ //------------------------------------------------------------------------------------------------------------------
  BME680menu = 0;
  Confirm = 0;
  // Display menu for BME680 once
  if (BME680menu == 0){
    tft.fillScreen(ST77XX_BLUE);
    tft.setCursor(1, 1);
    tft.println(" BME680 Sensor");
    tft.println("1 : Temperature Data"); 
    tft.println("2 : Pressure Data"); 
    tft.println("3 : Humidity Data"); 
    tft.println("4 : Gas Resistance Data"); 
    tft.println("5 : Altitidue Data");
    tft.println();
    tft.println("Please input your Option 1-5");
    BME680menu = 1;
  }
  // Wait until cancel button hit or selection is confirmed
  while (Confirm == 0 && Cancel == 0){
  Serial.println( "In Atmosphere sensor loop");
  if (Serial.available() > 0) {
    // wait for a user input
    Select_2 = Serial.read() - '0';
    if (Select_2 >0 && Select_2 <10)
    Select= Select_2;
  }
  Serial.println(Select);
  tft.setCursor(20, 100);
  tft.print("Current Selection:");
  tft.setCursor(100, 110);
  tft.print(Select);
  delay(1000);
  }
    // Option 1 
    if( Confirm == 1 && Select == 1 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Atmosphere sensor Data 1 loop");
    Serial.println( "In Atmosphere sensor Data 1 loop");
      }
          Cancel =0;
          delay(500);
    } 
    // Option 2
    else if (Confirm == 1 && Select == 2 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Atmosphere sensor Data 2 loop");
    Serial.println( "In Atmosphere sensor Data 2 loop");
      }
          Cancel =0;
          delay(500);
    } 
    // Option 3 
    else if (Confirm == 1 && Select == 3 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Atmosphere sensor Data 3 loop");
    Serial.println( "In Atmosphere sensor Data 3 loop");
      }
       Cancel =0;
       delay(500);
    } 
    // Option 4
    else if (Confirm == 1 && Select == 4 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Atmosphere sensor Data 4 loop");
    Serial.println( "In Atmosphere sensor Data 4 loop");
      }
      Cancel =0;
      delay(500);
    } 
    // Option 5
    else if (Confirm == 1 && Select == 5 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Atmosphere sensor Data 5 loop");
    Serial.println( "In Atmosphere sensor Data 5 loop");
      }
      Cancel =0;
      delay(500);
    } 
    // Cancel
    else if (Cancel == 1){
      Serial.print("Going back to main menu");
      Main_Menu= 0;
      Select = 1;
    } 
    // Wrong Input
    else {
    Serial.println( "Wrong Input");
    }

  } //------------------------------------------------------------------------------------------------------------------
} 
// Option 2 (Light Sensor Menu)
else if (Select == 2){  
  Select = 1;
  // Stay in Light Sensor menu until exit/cancel is chosen
  while (Cancel == 0){ //------------------------------------------------------------------------------------------------------------------
  Confirm = 0;
  // Display Light Sensor menu
  if (BH1750menu == 0){
  tft.fillScreen(ST77XX_BLUE);
  tft.setCursor(1, 1);
  tft.println(" BH1750 Sensor");
  tft.println("1 : Light Data"); 
  tft.println("2 : Light Data"); 
  tft.println();
  tft.println("Please input your Option 1-2");
  BH1750menu = 1;
  }
  // Wait until cancel button hit or selection is confirmed
  while (Confirm == 0 && Cancel == 0){
  Serial.println( "In Light sensor loop");
  if (Serial.available() > 0) {
    // wait for a user input
    Select_2 = Serial.read() - '0';
    if (Select_2 >0 && Select_2 <10)
    Select= Select_2;
  }
  Serial.println(Select);
  tft.setCursor(20, 100);
  tft.print("Current Selection:");
  tft.setCursor(100, 110);
  tft.print(Select);
  delay(1000);
  }
    // Option 1
    if( Confirm == 1 && Select == 1 && Cancel == 0){
       tft.fillScreen(ST77XX_BLUE);
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Light sensor Data 1 loop");
    Serial.println( "In Light sensor Data 1 loop");
      }
          Cancel =0;
          BH1750menu = 0;
          delay(500);
    } 
    // Option 2
    else if (Confirm == 1 && Select == 2 && Cancel == 0){
       tft.fillScreen(ST77XX_BLUE);
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Light sensor Data 2 loop");
    Serial.println( "In Light sensor Data 2 loop");
      }
          Cancel =0;
          BH1750menu = 0;
          delay(500);
    }  
    // Go Back/ cancel
    else if (Cancel == 1){
      Serial.print("Going back to main menu");
      Main_Menu= 0;
      Select =1;
    } 
    // Wrong input
    else {
    Serial.println( "Wrong Input");
    }

  } //------------------------------------------------------------------------------------------------------------------
}
// Option 3 (Incorrect selection)
else {
    Serial.println( "Wrong Input");
    }
 
}


void Menu1(){
  Confirm = 1;
  }


//Commented out below as these interrupts cause Arduino Due to not work
/*
void Menu2(){
Select = Select+1;
if (Select > 7){
Select = 1;
Serial.println(" Cannot go higher");
}
delay(1000);
}

void Menu3(){
Select = Select-1;
if (Select < 1){
Select = 1;
Serial.println(" Cannot go lower");
}
delay(1000);
}
*/

void Menu4(){
 Cancel = 1;
 }
