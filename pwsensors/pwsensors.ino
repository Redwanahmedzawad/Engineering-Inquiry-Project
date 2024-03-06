
#include "SdFat.h"
#include "SdFatConfig.h"

// Define file system for sd card
SdFs sd;
File file; // Change from FsFile to File

// Configuration for FAT16/FAT32 and exFAT.

// Chip select may be constant or RAM variable.
const uint8_t SD_CS_PIN = A3;
//
// Pin numbers in templates must be constants.
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN = 13;

// SdFat software SPI template
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(0), &softSpi)
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#define TFT_CS    10
#define TFT_RST   6 
#define TFT_DC    7 
#include "RTClib.h"
RTC_DS1307 rtc;
#define TFT_SCLK 13   
#define TFT_MOSI 11  
#include <avr/pgmspace.h>
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST); 
#include <BH1750.h>
#include <Wire.h>
#include <Wire.h>
#include <SPI.h>
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
int cmstr;
char a='0';
float dr=0;
float temp=0,humid=0,gas=0,press=0,light=0;
bool mn=true;
float preval = 0;
int Select = 0; // for incoming serial data
int Confirm = 0; // for incoming serial data
int Cancel = 0; // for incoming serial data
static int BME680menu = 0;
static int BH1750menu = 0;
static int Main_Menu= 0;
String menuls[]={"BME60 Sensor","BH1750FVI Sensor", "Serial Debugger", "Save Data"};
void setup() {
  //Arduino IDE Serial Monitor
  Serial.begin(9600);
     rtc.begin();
 rtc.adjust(DateTime(2023,10,13,05,41,0));
  Serial.println("Starting");
      tft.initR(INITR_BLACKTAB); 
       tft.setTextWrap(true);  // Don't wrap text to next line
  tft.setTextSize(1);  // large letters
  tft.setRotation(3);

  tft.fillScreen(ST77XX_BLACK);
  //SDI-12
  Serial1.begin(1200, SERIAL_7E1);      //SDI-12 UART, configures serial port for 7 data bits, even parity, and 1 stop bit
  pinMode(7, OUTPUT);                   //DIRO Pin
   pinMode(2,INPUT_PULLUP); // Button 1
  pinMode(3,INPUT_PULLUP); // Button 2
  pinMode(4,INPUT_PULLUP); // Button 3
  pinMode(5,INPUT_PULLUP); // Button 4


    if (!sd.begin(SD_CONFIG)) {
    Serial.println("SD card initialization failed!");
    sd.initErrorHalt();
    while (1);
  }

  // Open/create a file for writing
  if (!file.open("message.txt", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

  file.close(); // Release file
  //DIRO Pin LOW to Send to SDI-12
  digitalWrite(7, LOW); 
  Serial1.println("HelloWorld");
  delay(100);

  //HIGH to Receive from SDI-12
  digitalWrite(7, HIGH); 
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use
  // Wire.begin(D2, D1);

  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));
  Serial.println(F("BME680 test"));

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1){
    }
  }
   bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
    attachInterrupt(2,Menu1,RISING);
  attachInterrupt(5,Menu4,RISING); 
  attachInterrupt(4,selcol,RISING);
 // attachInterrupt(3,loop,RISING);
}

void loop(){
  //Receive SDI-12 over UART and then print to Serial Monitor
  menu();
 //sdicom();

}
void sdicom()
{
   char c = 'na';
  //tft.fillScreen(ST77XX_BLACK);
  //menu();
  DateTime now =rtc.now();
  String ftow;
  //menu();
if (Serial1.available()) {
    c = Serial1.read();

// Remove the first non byte that is received when entering a command
    if (c < 1){
      Serial.println("Byte removed");
    } 
 // Detect if the address query command is being issued   
    else if (c == '?'){
    c= Serial1.read();
    // Full address query command recognised/ execute command here
    Serial.println("HELE");
        if (c == '!'){
          Serial.println("Exclamation decetcted after ?");
          // Switch pin 7 to low in order to send Data
         sdiout(String(a));
         } else {
        Serial.println("...");      }
    }
    else if(c==a)
    {
      c = Serial1.read();
      Serial.println(c);
      if(c=='A')
      {
         c= Serial1.read();
         if(isDigit(c))
         {
          char d = c;
          c = Serial1.read();
          if(c == '!')
          {
            a = d;
            Serial.println(d);
            sdiout(String(a));
          }
         }
      }
      else if(c=='M')
      {
        c=Serial1.read();
        Serial.println(c);
        if(c=='!')
        {
          int tstart = millis();
          Serial.println("appp<CR><LF>");
          senval(tstart);
          String msg = String(a);
        int zc = 3 - String(dr).length();
        String dr1 = String(dr);
       
        String mout = "";
        for(int i=0;i<dr1.length();i++)
        {
          int chk =dr1[i]-'0';
          if(isDigit(dr1[i])&&dr1[i]=='0')
          {
            mout+=dr1[i];
          }
        }
        for(int i=0;i<dr1.length();i++)
        {
          if(isDigit(dr1[i])&&dr1[i]!='0')
          {
            mout+=dr1[i];
          }
        }
        String msg1 = String(a)+mout+"5";
        sdiout(msg1);
        Serial.println(msg1);
          //sdiout(msg1);
        }
      }
      else if(c == 'D')
      {
        c= Serial1.read();
        if(c == '1')
        {
         
          c = Serial1.read();
          if(c == '!')
          { 
            mn = true;
            senval(1);
           // dlcd();
            tempgraph2(temp,"Temp: ");
            
            String msg = String(a)+ String("+") + String(temp) + String("+") + String(press) + String("+") + String(humid) + String("+") + String(gas);
            sdiout(msg);
            ftow = String(now.hour())+":"+String(now.minute())+":"+String(now.second())+" "+msg;
             Serial.println(ftow);
            Serial.println(msg);
                 WriteSD(file, ftow);
     Serial.println(ftow);
     ReadSD(file);
            delay(500);
            pressgraph(press,"Pressure: ", " hPa/10000");
            delay(500);
            humidgraph(humid,"Humidity: ");
            delay(500);
            gasgraph(gas,"Gas: ");
          }
        }
      else if(c == '0')
        {
         
          c = Serial1.read();
          if(c == '!')
          { 
            mn = true;
            senval(1);
           // dlcd();
            tempgraph2(temp,"Temp: ");

            String msg = String(a)+ String("+") + String(temp) + String("+") + String(press) + String("+") + String(humid) + String("+") + String(gas)+"+"+String(light);
            sdiout(msg);
             String ftow = String(now.hour())+":"+String(now.minute())+":"+String(now.second())+" "+msg;
             Serial.println(ftow);
            Serial.println(msg);
            delay(500);
            pressgraph(press,"Pressure: ", " hPa/10000");
            delay(500);
            humidgraph(humid,"Humidity: ");
            delay(500);
            gasgraph(gas,"Gas: ");
          }
        }
        else if(c == '2')
        {
          senval(1);
          String msg = String(a)+"+"+String(light);
            ftow = String(now.hour())+":"+String(now.minute())+":"+String(now.second())+" "+msg;
               WriteSD(file, ftow);
     Serial.println(ftow);
     ReadSD(file);
          sdiout(msg);
          lightgraph(light, "Light:");
          Serial.println(ftow);
        }
      }
      else if(c == 'I')
      {
        c=Serial1.read();
        if(c=='!')
        {
          String msg = String(a)+String("14")+ "ENG20009"+"103501849";
          sdiout(msg);
          Serial.println(msg);
        }
      }
    }
    senval(1);
    String msg1 = String(a)+ String("+") + String(temp) + String("+") + String(press) + String("+") + String(humid) + String("+") + String(gas);

   
    ftow = String(now.hour())+":"+String(now.minute())+":"+String(now.second())+" "+msg1;
     //WriteSD(file, ftow);
     //Serial.println(ftow);
     //ReadSD(file);
    // Delay to remove noise
  delay(100);
}

}
void senval(int t)
{
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");
  temp = bme.temperature;
  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");
  press=bme.pressure;
  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  humid = bme.humidity;
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");
  gas = bme.gas_resistance;
  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  light = lightMeter.readLightLevel();
  
  Serial.print("Light: ");
  Serial.print(light);
  Serial.println(" lx");
  delay(300);
  int ltime = millis();
  dr = (ltime -t)/1000;
  Serial.println(dr);
}
void sdiout(String data)
{
    digitalWrite(7, LOW); 
       
          Serial1.println(data);
          delay (500);
          // Switch pin 7 to High in order to receive data again
          digitalWrite(7, HIGH); 
          delay(100);
}
void dlcd()
{
  
  /*Function to display the temperature graph using data from BME sensor*/
 tft.fillScreen(ST77XX_BLACK);
 tft.setCursor(2, 2);
tft.setTextSize(2);
tft.print("Tempareture: ");
tft.print(temp);
tft.println(" *C");
//tft.setCursor(2,4);
tft.print("Pressure: ");
tft.print(press);
tft.println(" hPa");
tft.print("Humidity: ");
tft.print(humid);
tft.println("%");
tft.print("Gas: ");
tft.print(gas);
tft.println(" KOhms");
  delay(500);


}
void tempgraph2(float gtod , String stod)

{
    tft.fillScreen(ST77XX_BLACK);
 tft.setTextColor(ST77XX_GREEN);
    tft.fillRect(0,85,1000,1,ST77XX_WHITE);
    tft.fillRect(3,0,1,1000,ST77XX_WHITE);
   tft.setCursor(150, 82);
    tft.print(">");
    tft.setCursor(110, 100);
    tft.print("time(s)");
    tft.setCursor(1,82);
    tft.print("<");
    tft.setCursor(1, 3);
    tft.print("^");
    tft.setCursor(25,120);
    tft.print("*C");
    tft.setCursor(1,120);
    tft.print("v");
   
   // tft.print(stod);
    int a = 0;
    for(int i =0;i < 8;i++)
    {
      
      tft.setCursor(i*20, 82);
      tft.print("|");
      tft.setCursor(i*20,88);
    //  tft.setTextSize(.5);
      tft.print(a);
      a++;
    }
    int b=-20;
    for(int j=0;j<7;j++)
    {
      tft.setCursor(1, j*20);
      tft.print("-");
      tft.setCursor(5,(6-j)*20+2);
      tft.print(b);
      b+=10;
    }
   
}
void pressgraph(float gtod , String stod, String unit)

{
    tft.fillScreen(ST77XX_BLACK);
     tft.setTextColor(ST77XX_GREEN);
    tft.fillRect(0,85,1000,1,ST77XX_WHITE);
    tft.fillRect(3,0,1,1000,ST77XX_WHITE);
     tft.setCursor(150, 82);
    tft.print(">");
    tft.setCursor(110, 100);
    tft.print("time(s)");
    tft.setCursor(1,82);
    tft.print("<");
    tft.setCursor(1, 3);
    tft.print("^");
    tft.setCursor(25,120);
    tft.print(unit);
    tft.setCursor(1,120);
    tft.print("v");
   
   // tft.print(stod);
    int a = 0;
    for(int i =0;i < 8;i++)
    {
      
      tft.setCursor(i*20, 82);
      tft.print("|");
      tft.setCursor(i*20,88);
    //  tft.setTextSize(.5);
      tft.print(a);
      a++;
    }
    int b=-20;
    for(int j=0;j<7;j++)
    {
      tft.setCursor(1, j*20);
      tft.print("-");
      tft.setCursor(5,(6-j)*20+2);
      tft.print(b);
      b+=10;
    }
}
void humidgraph(float gtod , String stod)

{
       tft.fillScreen(ST77XX_BLACK);
     tft.setTextColor(ST77XX_GREEN);
    tft.fillRect(0,85,1000,1,ST77XX_WHITE);
    tft.fillRect(3,0,1,1000,ST77XX_WHITE);
     tft.setCursor(150, 82);
    tft.print(">");
    tft.setCursor(110, 100);
    tft.print("time(s)");
    tft.setCursor(1,82);
    tft.print("<");
    tft.setCursor(1, 3);
    tft.print("^");
    tft.setCursor(25,120);
    tft.print("%");
    tft.setCursor(1,120);
    tft.print("v");
   
   // tft.print(stod);
    int a = 0;
    for(int i =0;i < 8;i++)
    {
      
      tft.setCursor(i*20, 82);
      tft.print("|");
      tft.setCursor(i*20,88);
    //  tft.setTextSize(.5);
      tft.print(a);
      a++;
    }
    int b=-40;
    for(int j=0;j<7;j++)
    {
      tft.setCursor(1, j*20);
      tft.print("-");
      tft.setCursor(5,(6-j)*20+2);
      tft.print(b);
      b+=20;
    }
}

void gasgraph(float gtod , String stod)

{
         tft.fillScreen(ST77XX_BLACK);
     tft.setTextColor(ST77XX_GREEN);
    tft.fillRect(0,85,1000,1,ST77XX_WHITE);
    tft.fillRect(3,0,1,1000,ST77XX_WHITE);
     tft.setCursor(150, 82);
    tft.print(">");
    tft.setCursor(110, 100);
    tft.print("time(s)");
    tft.setCursor(1,82);
    tft.print("<");
    tft.setCursor(1, 3);
    tft.print("^");
    tft.setCursor(25,120);
    tft.print(" KOhms/3000");
    tft.setCursor(1,120);
    tft.print("v");
   
   // tft.print(stod);
    int a = 0;
    for(int i =0;i < 8;i++)
    {
      
      tft.setCursor(i*20, 82);
      tft.print("|");
      tft.setCursor(i*20,88);
    //  tft.setTextSize(.5);
      tft.print(a);
      a++;
    }
    int b=-60;
    for(int j=0;j<7;j++)
    {
      tft.setCursor(1, j*20);
      tft.print("-");
      tft.setCursor(5,(6-j)*20+2);
      tft.print(b);
      b+=30;
    }
}
void lightgraph(float gtod, String stod)
{
         tft.fillScreen(ST77XX_BLACK);
     tft.setTextColor(ST77XX_GREEN);
    tft.fillRect(0,85,1000,1,ST77XX_WHITE);
    tft.fillRect(3,0,1,1000,ST77XX_WHITE);
     tft.setCursor(150, 82);
    tft.print(">");
    tft.setCursor(110, 100);
    tft.print("time(s)");
    tft.setCursor(1,82);
    tft.print("<");
    tft.setCursor(1, 3);
    tft.print("^");
    tft.setCursor(25,120);
    tft.print(" lux");
    tft.setCursor(1,120);
    tft.print("v");
   
   // tft.print(stod);
    int a = 0;
    for(int i =0;i < 8;i++)
    {
      
      tft.setCursor(i*20, 82);
      tft.print("|");
      tft.setCursor(i*20,88);
    //  tft.setTextSize(.5);
      tft.print(a);
      a++;
    }
    int b=-20;
    for(int j=0;j<7;j++)
    {
      tft.setCursor(1, j*20);
      tft.print("-");
      tft.setCursor(5,(6-j)*20+2);
      tft.print(b);
      b+=10;
    }
}
void WriteSD(File file, String message) {
  Serial.println("---   Saving To File   ---");

  file.open("message.txt", O_RDWR | O_APPEND);
  //file.rewind(); // Go to file position 0
  file.println("Message written using SoftwareSPI");
  file.println(message);
  file.close();
}

void ReadSD(File file) {
  Serial.println("--- Reading From file! ---");

  file.open("message.txt", O_RDWR);
  file.seek(0); // Go to char 0
  String contents = file.readString();
  Serial.println(contents);
  file.close();
}
void goback()
{
  tft.fillScreen(ST77XX_BLUE);
      tft.println("  Main Menu");
    tft.setCursor(5,10);
    tft.println(" 1 : BME680 Sensor");
    tft.setCursor(5,20);
    tft.println(" 2 : BH1750FVI Sensor");
    tft.setCursor(5,30);
    tft.println(" 3 : Serial Debugger");
    tft.setCursor(5,40);
     tft.println(" 4 : Save to Memory");
    
    tft.println();
    tft.println(" Please input your Option 1-4");
}
void menu()
{

  int Select_2 = 1;
  Confirm =0; //
Cancel =0;
 DateTime now =rtc.now();
 // Serial.println(Select); // Show current value of Select in Serial Monitor
tft.fillRect(1, 10, 50, 20, ST77XX_BLACK);
// Wait until a selection is confirmed.
while (Confirm == 0){
 // Print out menu only once
  if (Main_Menu == 0){
  goback();
    Main_Menu = 1; // Main menu will only show once
  }
 // Check for change in selection
 /* if (Serial.available() > 0) {
    // wait for a user input
    Select_2 = Serial.read() - '0';
    if (Select_2 >0 && Select_2 <10)
    Select= Select_2;
  }*/
  // Display selection value in TFT
  Serial.println(Select);
  tft.setCursor(20, 100);
  tft.print("Current Selection:");
  tft.setCursor(100, 110);
  tft.print("");
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
   // tft.setCursor(5,10);
    tft.println(" BME680 Sensor");
    tft.setCursor(5,10);
    tft.println("1 : Temperature Data"); 
    tft.setCursor(5,20);
    tft.println("2 : Pressure Data"); 
    tft.setCursor(5,30);
    tft.println("3 : Humidity Data"); 
    tft.setCursor(5,40);
    tft.println("4 : Gas Resistance Data"); 
    //tft.println("5 : Altitidue Data");
    tft.println();
    tft.println("Please input your Option 1-4");
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
  tft.print(" ");
  tft.setCursor(100, 110);
  tft.print(Select);
  delay(1000);
  }
    // Option 1 
    if( Confirm == 1 && Select == 1 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      tempgraph2(temp,"Temp: ");
      int graph =0;
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Atmosphere sensor Data 1 loop");
    Serial.println( "In Atmosphere sensor Data 1 loop");
    senval(1);
       if(graph<7){
    tft.fillRect(graph*20+2,86-temp,20,temp,ST77XX_WHITE);
    }
    graph++;
    delay(1000);
      }
          Cancel =0;
             
          delay(500);
    } 
    // Option 2
    else if (Confirm == 1 && Select == 2 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE); int graph =0;
      pressgraph(press,"Pressure: "," hPa/10000");
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Atmosphere sensor Data 2 loop");
    Serial.println( "In Atmosphere sensor Data 2 loop");
    senval(1);
    if(graph<7){
    tft.fillRect(graph*20+2,86-press/10000,20,press/10000,ST77XX_WHITE);
    }
    graph++;
      }
       
          Cancel =0;
          delay(500);
    } 
    // Option 3 
    else if (Confirm == 1 && Select == 3 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      humidgraph(humid, " %");
      int graph =0;
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Atmosphere sensor Data 3 loop");
    Serial.println( "In Atmosphere sensor Data 3 loop");
    senval(1);
    if(graph<7)
    {
      tft.fillRect(graph*20+2,86-humid,20,humid,ST77XX_WHITE);
    }
    graph++;
      }
       Cancel =0;
       delay(500);
    } 
    // Option 4
    else if (Confirm == 1 && Select == 4 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      gasgraph(gas, " KOhms");
      int graph=0;
      while (Cancel == 0){
        tft.setCursor(1, 1);
        tft.print("In Atmosphere sensor Data 4 loop");
    Serial.println( "In Atmosphere sensor Data 4 loop");
    senval(1);
    if(graph<7)
    {
      tft.fillRect(graph*20+2,86-gas/3000,20,gas/3000,ST77XX_WHITE);
    }
    graph++;
      }
      Cancel =0;
      delay(500);
    } 
    // Cancel
    else if (Cancel == 1){
      Serial.print("Going back to main menu");
      Main_Menu= 0;
      Select = 1;
      goback();
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
 //------------------------------------------------------------------------------------------------------------------
  Confirm = 0;
  // Display Light Sensor menu
 lightgraph(light,"Light: ");
 int graph =0;
  // Wait until cancel button hit or selection is confirmed
  while (Cancel == 0){
   tft.setCursor(1, 1);
        tft.print("In Light sensor Data loop");
    Serial.println( "In Atmosphere sensor Data 3 loop");
    senval(1);
    if(graph<7){
    tft.fillRect(graph*20+2,86-light,20,light,ST77XX_WHITE);
    }
    graph++;
  }
    // Option 1
  
    // Option 2 
    // Go Back/ cancel
    if (Cancel == 1){
      Serial.print("Going back to main menu");
      
      Main_Menu= 0;
      goback();
      Select =1;
    } 
    // Wrong input
    else {
    Serial.println( "Wrong Input");
    }

  //------------------------------------------------------------------------------------------------------------------
}
else if(Select == 3)
{

         tft.fillScreen(ST77XX_BLUE);
          tft.setCursor(1, 1);
        tft.print("In Serial Debugger");
      while (Cancel == 0){
    sdicom();
      }
          if (Cancel == 1){
      Serial.print("Going back to main menu");
      
      Main_Menu= 0;
      goback();
      Select =1;
    } 
}
else if(Select == 4)
{
  while(Cancel == 0){
            senval(1);
          String msg = String(a)+"+"+String(light);
           String ftow = String(now.hour())+":"+String(now.minute())+":"+String(now.second())+" "+msg;
               WriteSD(file, ftow);
              String msg1 = String(a)+ String("+") + String(temp) + String("+") + String(press) + String("+") + String(humid) + String("+") + String(gas);
              ftow = String(now.hour())+":"+String(now.minute())+":"+String(now.second())+" "+msg1;
               WriteSD(file, ftow);
               tft.fillScreen(ST77XX_BLUE);
               tft.print("Saving");
  }
                         if (Cancel == 1){
      Serial.print("Going back to main menu");
      
      Main_Menu= 0;
      goback();
      Select =1;
    } 
              
}
// Option 3 (Incorrect selection)

else {
    Serial.println( "Wrong Input");
    }
}

void Menu1(){
  Confirm = 1;
  }
void Menu4(){
 Cancel = 1;
 }
 void selcol()
 {
  tft.fillCircle(1, Select*10, 5, ST77XX_BLUE);
  if(Select == 4){
    Select = 0;
  }
  Select++;
  tft.fillCircle(2, Select*10+2, 2, ST77XX_YELLOW);
 // delay(300);
 // tft.setTextColor(ST77XX_RED);
  //tft.setCursor(1, Select*10);
 // tft.println(menuls[Select]);
 }