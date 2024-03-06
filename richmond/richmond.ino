//https://forum.arduino.cc/t/timer-interrupt-on-arduino-zero-board/409166/6
boolean toggle1 = 0;
 int Menu =0;
int Change = 0;

#include <DFRobot_BMX160.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define BUTTON_PIN1 2
#define BUTTON_PIN2 3
#define BUTTON_PIN3 4

#define TFT_CS    10
#define TFT_RST   6 
#define TFT_DC    7 

#define TFT_SCLK 13   
#define TFT_MOSI 11   
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
DFRobot_BMX160 bmx160;

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising");
    pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);
delay(100);

  if (bmx160.begin() != true){
    Serial.println("init false");
    while(1);
  }
  bmx160.setGyroRange(eGyroRange_500DPS);
    tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  //rotate screen to landscape
  tft.setRotation(2);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_GREEN);
    tft.fillRect(0,85,1000,1,ST77XX_WHITE);
    tft.fillRect(60,0,1,1000,ST77XX_WHITE);
    tft.setCursor(110, 90);
    tft.print("-<");
    tft.setCursor(65, 150);
    tft.print("x");
    tft.setCursor(60,85);

  pinMode(11, OUTPUT); // setup output
  
  PORT->Group[PORTA].DIRSET.reg = PORT_PA06;       // Set-up digital pin D8 as an output
  
  // Set up the generic clock (GCLK4) used to clock timers
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |          // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
                    GCLK_GENDIV_ID(4);             // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                      GCLK_GENCTRL_GENEN |         // Enable GCLK4
                      GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                      GCLK_GENCTRL_ID(4);          // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

  // Feed GCLK4 to TC4 and TC5
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
                      GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                      GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

  TC4->COUNT16.CC[0].reg = 0xB71A;                 // Set the TC4 CC0 register as the TOP value in match frequency mode
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  //NVIC_DisableIRQ(TC4_IRQn);
  //NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC4_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  TC4->COUNT16.INTFLAG.bit.OVF = 1;                        // Clear the interrupt flags
  TC4->COUNT16.INTENSET.bit.OVF = 1;                       // Enable TC4 interrupts
 
  TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 |   // Set prescaler to 1024, 48MHz/1024 = 46.875kHz
                            TC_CTRLA_WAVEGEN_MFRQ |        // Put the timer TC4 into match frequency (MFRQ) mode
                            TC_CTRLA_ENABLE;               // Enable TC4
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);   

}
void loop() {
  // put your main code here, to run repeatedly:
  
if (!digitalRead(2)){
  Menu = 1;
}
if (!digitalRead(3)){
Menu = 2;
}
if (!digitalRead(4)){
Menu = 3;
}
// Serial.println(Menu);
}


void TC4_Handler()                                 // Interrupt Service Routine (ISR) for timer TC4
{     
  sBmx160SensorData_t Omagn, Ogyro, Oaccel;
   bmx160.getAllData(&Omagn, &Ogyro, &Oaccel); 
  // Check for overflow (OVF) interrupt

   tft.setRotation(2);


 if (Change != Menu)
 {
    Change = Menu;
    Serial.println(" The Option has been changed");
 }

  if (TC4->COUNT16.INTFLAG.bit.OVF && TC4->COUNT16.INTENSET.bit.OVF)             
  {
    // Put your timer overflow (OVF) code here:     
    // ...
 //   PORT->Group[PORTA].OUTTGL.reg = PORT_PA06;     // Toggle the D8 pin HIGH LOW
  
   if (toggle1){
    Serial.print(Menu);

// Magnometer value to print in Serial Monitor + display graph in TFT
if(Menu == 1){
   Serial.print("M ");
  Serial.print("X: "); Serial.print(Omagn.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Omagn.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Omagn.z); Serial.print("  ");
  Serial.println("uT");

  tft.fillRect(60,85,Omagn.y/3,Omagn.x/3,ST77XX_RED);
} 

// Gyrometer value to print in Serial Monitor + display graph in TFT
else if (Menu == 2){

    Serial.print("G ");
  Serial.print("X: "); Serial.print(Ogyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Ogyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Ogyro.z); Serial.print("  ");
  Serial.println("g");

  tft.fillRect(60,85,Ogyro.y*2,Ogyro.x*2,ST77XX_BLUE);

} 

// Acceleromter value to print in Serial Monitor + display graph in TFT
else if (Menu ==3){
    Serial.print("A ");
  Serial.print("X: "); Serial.print(Oaccel.x    ); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Oaccel.y    ); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Oaccel.z    ); Serial.print("  ");
  Serial.println("m/s^2");

  tft.fillRect(60,85,Oaccel.y*3,Oaccel.x*3,ST77XX_GREEN);
}
Serial.println("");
    toggle1 = 0;
  }
  else{
// Clear display every 1 second
    toggle1 = 1;
  // Menu = Menu+1;
  tft.setRotation(2);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_GREEN);
    tft.fillRect(0,85,1000,1,ST77XX_WHITE);
    tft.fillRect(60,0,1,1000,ST77XX_WHITE);
    tft.setCursor(110, 90);
    tft.print("-<");
    tft.setCursor(65, 150);
    tft.print("x");
    tft.setCursor(60,85);
  }

    TC4->COUNT16.INTFLAG.bit.OVF = 1;              // Clear the OVF interrupt flag
  }
}

/*
void TC3_Handler()
{
  Serial.println("test");  
  if (toggle1){
    digitalWrite(11,HIGH);
    toggle1 = 0;
  }
  else{
    digitalWrite(11,LOW);
    toggle1 = 1;
  }
}
*/