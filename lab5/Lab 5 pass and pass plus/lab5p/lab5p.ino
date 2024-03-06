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
String li[] PROGMEM= {" 103501849", "Md Redwan Ahmed Zawad", "ENG20009", "Engineering Technology Inquiry Project", "Semester 1", "2023"};
String buf[6];
const char std_id[] PROGMEM = {"103501849"};
const char std_name[]PROGMEM = {"Md Redwan Ahmed Zawad"};
const char uni[] PROGMEM = {"ENG20009"};
const char Full_uni[] PROGMEM = "Engineering Technology Inquiry Project";
const char sem[] PROGMEM = "Semester 2";
const char yer[] PROGMEM = "2023";
char c_ar[1000];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 // strcpy(buf.begin(),c_ar);
  buf[0] = constro(std_id);
  buf[1]= constro(std_name);
  buf[2]= constro(uni);
  buf[3]= constro(Full_uni);
  buf[4]= constro(sem);
  buf[5]= constro(yer);

  tft.initR(INITR_BLACKTAB); 
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println(buf[0]);
}
String constro(const char chh[])
{
  String str;
  for(int i=0;i<strlen(chh);i++)
  {
    char c=pgm_read_byte_near(chh+i);
    str+=c;
    Serial.println(c);
  }
  return str;
}
void loop() {
  // put your main code here, to run repeatedly:
 for(int i=0;i<(sizeof(buf)/sizeof(String));i++){
    for (int j=0;j<=(sizeof(buf)/sizeof(String))*(sizeof(buf)/sizeof(String));j++)
    {
     tft.fillScreen(ST77XX_BLACK);

      tft.setCursor(128-j,0);
      tft.setTextSize(2);
     // tft.setTextColor(ST77XX_WHITE);
      tft.print(buf[i]);
      //tft.display();
      delay(100);
    }
    delay(250);
 }
  
}

