#include "SdFat.h"
#include "SdFatConfig.h"
#include <DueTimer.h>

SdFs sd;
File file;


const uint8_t SD_CS_PIN = A3;

const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN = 13;


SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(0), &softSpi)
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
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

Adafruit_BME680 bme; 
int cmstr;
char a='0';
float dr=0;
float temp=0,humid=0,gas=0,press=0,light=0;
bool mn=true;
float preval = 0;
static int Select = 1; 
int Confirm = 0; 
int Cancel = 0; 
int Disable_stepper = 0;
static int BME680menu = 0;
static int BH1750menu = 0;
static int Main_Menu= 0;
String menuls[]={"BME60 Sensor","BH1750FVI Sensor", "Serial Debugger", "Save Data"};
void setup() {

  Serial.begin(9600);
     rtc.begin();
 rtc.adjust(DateTime(2023,10,13,05,41,0));
  Serial.println("Starting");
      tft.initR(INITR_BLACKTAB); 
       tft.setTextWrap(true);  
  tft.setTextSize(1);  
  tft.setRotation(3);

  tft.fillScreen(ST77XX_BLACK);
  //SDI-12
  Serial1.begin(1200, SERIAL_7E1);      
  pinMode(7, OUTPUT);                   
   pinMode(2,INPUT_PULLUP); 
  pinMode(3,INPUT_PULLUP); 
  pinMode(4,INPUT_PULLUP); 
  pinMode(5,INPUT_PULLUP); 


    if (!sd.begin(SD_CONFIG)) {
    Serial.println("SD card initialization failed!");
    sd.initErrorHalt();
    while (1);
  }


  if (!file.open("message.txt", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

  file.close(); 
  
  digitalWrite(7, LOW); 
  Serial1.println("HelloWorld");
  delay(100);


  digitalWrite(7, HIGH); 
  Wire.begin();


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
    attachInterrupt(3,Menu1,RISING);
  attachInterrupt(5,Menu4,RISING); 
  attachInterrupt(4,selcol,RISING);

   Timer3.attachInterrupt(forwrite);
  Timer3.start(600000000); 
}

void loop(){

  menu();

}
void forwrite()
{
  DateTime now =rtc.now();
      senval(1);
       String msg = String(a)+ String("+") + String(temp) + String("+") + String(press) + String("+") + String(humid) + String("+") + String(gas)+"+"+String(light);

        String    ftow = String(now.hour())+":"+String(now.minute())+":"+String(now.second())+" "+msg;
               WriteSD(file, ftow);
}
void menu()
{

  int Select_2 = 1;
  Confirm =0; 
  Cancel =0;
  DateTime now =rtc.now();

  tft.fillRect(1, 10, 50, 20, ST77XX_BLACK);

  while (Confirm == 0){

  if (Main_Menu == 0){
  MainMenu();
    Main_Menu = 1; 
  }
 // Check for change in selection
 /* if (Serial.available() > 0) {
    // wait for a user input
    Select_2 = Serial.read() - '0';
    if (Select_2 >0 && Select_2 <10)
    Select= Select_2;
  }*/

  Serial.println(Select);
  tft.setCursor(20, 100);
  tft.print("Current Selection:");
  tft.setCursor(100, 110);
  tft.print("");
  tft.setCursor(100, 110);
  tft.print(Select);
  delay(1000);
}
  Cancel = 0; 
if (Select == 1)
{  

  while (Cancel == 0){ 
  BME680menu = 0;
  Confirm = 0;

  if (BME680menu == 0){
    tft.fillScreen(ST77XX_BLUE);
    tft.setTextColor(ST77XX_WHITE);
    tft.fillCircle(2, Select*10+2, 2, ST77XX_YELLOW);
    tft.setCursor(1, 1);
    tft.println(" BME680 Sensor");
    tft.setCursor(5,10);
    tft.println("1 : Temperature Data"); 
    tft.setCursor(5,20);
    tft.println("2 : Pressure Data"); 
    tft.setCursor(5,30);
    tft.println("3 : Humidity Data"); 
    tft.setCursor(5,40);
    tft.println("4 : Gas Resistance Data"); 
    tft.println();
    tft.println("Please input your Option 1-4");
    BME680menu = 1;
  }
  while (Confirm == 0 && Cancel == 0){
  Serial.println( "In Atmosphere sensor loop");
  Serial.println(Select);
  tft.setCursor(20, 100);
  tft.print("Current Selection:");
  tft.setCursor(100, 110);
  tft.print(" ");
  tft.setCursor(100, 110);
  tft.print(Select);
  delay(1000);
  }
    if( Confirm == 1 && Select == 1 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      tempgraph2(temp,"Temp: ");
      int graph =0;
      Disable_stepper = 1;
      while (Cancel == 0){
        tft.setCursor(30, 1);
        tft.print("Temperature Graph");
    Serial.println( "In Atmosphere menu 1 loop");
    senval(1);
       if(graph<7){
    tft.fillRect(graph*20+2,86-temp,20,temp,ST77XX_WHITE);
    }
    graph++;
    delay(1000);
      }
      Disable_stepper = 0;
      Cancel =0;   
      delay(500);
    } 
    else if (Confirm == 1 && Select == 2 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE); 
      int graph =0;
      Disable_stepper = 1;
      pressgraph(press,"Pressure: "," hPa/10000");
      while (Cancel == 0){
        tft.setCursor(30, 1);
        tft.print("Pressure Graph");
    Serial.println( "In Atmosphere menu 2 loop");
    senval(1);
    if(graph<7){
    tft.fillRect(graph*20+2,86-press/10000,20,press/10000,ST77XX_WHITE);
    }
    graph++;
      }
      Disable_stepper = 0;
      Cancel =0;
      delay(500);
    } 
    else if (Confirm == 1 && Select == 3 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      humidgraph(humid, " %");
      int graph =0;
      Disable_stepper = 1;
      while (Cancel == 0){
        tft.setCursor(30, 1);
        tft.print("Humidity Graph");
    Serial.println( "In Atmosphere menu 3 loop");
    senval(1);
    if(graph<7)
    {
      tft.fillRect(graph*20+2,86-humid,20,humid,ST77XX_WHITE);
    }
    graph++;
      }
      Disable_stepper = 0;
      Cancel =0;
      delay(500);
    } 
    else if (Confirm == 1 && Select == 4 && Cancel == 0){
      tft.fillScreen(ST77XX_BLUE);
      gasgraph(gas, " KOhms");
      int graph=0;
      Disable_stepper = 1;
      while (Cancel == 0){
        tft.setCursor(30, 1);
        tft.print("Gas Graph");
    Serial.println( "In Atmosphere menu 4 loop");
    senval(1);
    if(graph<7)
    {
      tft.fillRect(graph*20+2,86-gas/3000,20,gas/3000,ST77XX_WHITE);
    }
    graph++;
      }
      Disable_stepper = 0;
      Cancel =0;
      delay(500);
    } 
    else if (Cancel == 1){
      Serial.print("Going back to main menu");
      Main_Menu= 0;
      Select = 1;
    } 
    else {
    Serial.println( "Wrong Input");
    }
  }
} 
else if (Select == 2)
{
  Select = 1;
  Confirm = 0;
  while (Cancel == 0){ 
  Confirm = 0;
  if (BH1750menu == 0){
    tft.fillScreen(ST77XX_BLUE);
    tft.setTextColor(ST77XX_WHITE);
    tft.fillCircle(2, Select*10+2, 2, ST77XX_YELLOW);
    tft.setCursor(1, 1);
    tft.println(" BH1750 Sensor");
    tft.setCursor(5,10);
    tft.println("1 : Light Data"); 
    tft.println();
    tft.println("Please input your Option");
    BH1750menu = 1;
  }

  while (Confirm == 0 && Cancel == 0){
  Serial.println( "In Light sensor loop");

  Serial.println(Select);
  tft.setCursor(20, 100);
  tft.print("Current Selection:");
  tft.setCursor(100, 110);
  tft.print(" ");
  tft.setCursor(100, 110);
  tft.print(Select);
  delay(1000);
  }

    if( Confirm == 1 && Select == 1 && Cancel == 0){
  tft.fillScreen(ST77XX_BLUE);
  lightgraph(light,"Light: ");
  int graph =0;
  Disable_stepper = 1;
  while (Cancel == 0){
   tft.setCursor(30, 1);
    tft.print("Light data");
    Serial.println( "In Light data loop");
    senval(1);
    if(graph<7){
    tft.fillRect(graph*20+2,86-light,20,light,ST77XX_WHITE);
    }
    graph++;
    delay(1000);
      }
      Disable_stepper = 0;
      Cancel =0;
      delay(500);
      BH1750menu = 0;
    } else if (Cancel == 1){
      Serial.print("Going back to main menu");
      Main_Menu= 0;
      Select = 1;
    }   else{
    Serial.println( "Wrong Input");
    }

}
BH1750menu = 0;
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
               tft.setCursor(1, 1);
               tft.println("Saving");
  }
                         if (Cancel == 1){
      Serial.print("Going back to main menu");   
      Main_Menu= 0;
      Select =1;
    } 
              
}

else {
    Serial.println( "Wrong Input");
    }
}

void MainMenu()
{
  tft.fillScreen(ST77XX_BLUE);
  tft.setCursor(1, 1);
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
    tft.fillCircle(2, Select*10+2, 2, ST77XX_YELLOW);
}

void sdicom()
{
   char c = 'na';
  DateTime now =rtc.now();
  String ftow;
if (Serial1.available()) {
    c = Serial1.read();


    if (c < 1){
      Serial.println("Byte removed");
    } 
  
    else if (c == '?'){
    c= Serial1.read();

        if (c == '!'){
          Serial.println("Exclamation decetcted after ?");

         sdiout(String(a));
         } else {
        Serial.println("Incorrect input");      }
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

          digitalWrite(7, HIGH); 
          delay(100);
}
void dlcd()
{ 

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
      tft.print(b*2);
      b+=10;
    }
}

void WriteSD(File file, String message) {
  Serial.println("---   Saving To File   ---");
  file.open("message.txt", O_RDWR | O_APPEND);
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

  if (BH1750menu == 1){
    Select = 0;
  }
  if (Disable_stepper == 1){
    Serial.println( "Stepper is Disbaled");
  } 
  else
  Select++;

  tft.fillCircle(2, Select*10+2, 2, ST77XX_YELLOW);
  tft.fillRect(100,110,20,20,ST77XX_BLUE);
 }