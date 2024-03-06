#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>


#define TFT_CS    10
#define TFT_RST   6 
#define TFT_DC    7 

#define TFT_SCLK 13   
#define TFT_MOSI 11 
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

int Select = 1; // for incoming serial data
int Confirm = 0; // for incoming serial data
int Cancel = 0; // for incoming serial data
void setup() {
  Serial.begin(9600);  // start serial communication at 9600 baud

/*
  pinMode(2,INPUT_PULLUP); // Button 1
  pinMode(3,INPUT_PULLUP); // Button 2
  pinMode(4,INPUT_PULLUP); // Button 3
  pinMode(5,INPUT_PULLUP); // Button 4
  attachInterrupt(2,Menu1,RISING);
  attachInterrupt(3,Menu2,RISING);
  attachInterrupt(4,Menu3,RISING);
  attachInterrupt(5,Menu4,RISING); 
/*
  pinMode(11, OUTPUT); // First LED from top of LED BAR
  pinMode(10, OUTPUT); // Second LED from top of LED BAR
  pinMode(9, OUTPUT); // Third LED from top of LED BAR
*/
  tft.initR(INITR_BLACKTAB);   
  tft.fillScreen(ST77XX_BLUE);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);  // White on black
  tft.setTextWrap(true);  // Don't wrap text to next line
  tft.setTextSize(1);  // large letters
  tft.setRotation(3); // horizontal display

}

void loop() {
// static int Select_2 = 0;
//int Exit = 0 ;
int Main_Menu= 0;
int BME680menu = 0;
int BH1750menu = 0;

Serial.println(Select);

while (Confirm == 0){

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

Serial.println(Select);
delay(1000);
}

if (Select == 1) 
{  

  while (Cancel == 0){ //------------------------------------------------------------------------------------------------------------------
  BME680menu = 1;
  Confirm = 0;
  
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

while (Confirm == 0 && Cancel == 0){
Serial.println( "In Atmosphere sensor loop");
Serial.println(Select);
delay(1000);
}

    if( Confirm == 1 && Select == 1 && Cancel == 0){
      Cancel =0;
      while (Cancel == 0){
    Serial.println( "In Atmosphere sensor Data 1 loop");
      }
          Cancel =0;
    } else if (Confirm == 1 && Select == 2 && Cancel == 0){
      Cancel =0;
      while (Cancel == 0){
    Serial.println( "In Atmosphere sensor Data 2 loop");
      }
          Cancel =0;
    } else if (Confirm == 1 && Select == 3 && Cancel == 0){
      Cancel =0;
      while (Cancel == 0){
    Serial.println( "In Atmosphere sensor Data 3 loop");
      }
       Cancel =0;
    } else if (Confirm == 1 && Select == 4 && Cancel == 0){
      Cancel =0;
      while (Cancel == 0){
    Serial.println( "In Atmosphere sensor Data 4 loop");
      }
      Cancel =0;
    } else if (Confirm == 1 && Select == 5 && Cancel == 0){
      Cancel =0;
      while (Cancel == 0){
    Serial.println( "In Atmosphere sensor Data 5 loop");
      }
      Cancel =0;
    } else if (Cancel == 1){
      Serial.print("Going back to main menu");
    } else {
    Serial.println( "Wrong Input");
    }

  } //------------------------------------------------------------------------------------------------------------------
} else if (Select == 2){  

  while (Cancel == 0){ //------------------------------------------------------------------------------------------------------------------
  BH1750menu = 1;
  Confirm = 0;
  
tft.fillScreen(ST77XX_BLUE);
tft.setCursor(1, 1);
tft.println(" BH1750 Sensor");
tft.println("1 : Light Data"); 
tft.println();
tft.println("Please input your Option 1-2");

while (Confirm == 0 && Cancel == 0){
Serial.println( "In Light sensor loop");
Serial.println(Select);
delay(1000);
}

    if( Confirm == 1 && Select == 1 && Cancel == 0){
      Cancel =0;
      while (Cancel == 0){
    Serial.println( "In Atmosphere sensor Data 1 loop");
      }
          Cancel =0;
    } else if (Confirm == 1 && Select == 2 && Cancel == 0){
      Cancel =0;
      while (Cancel == 0){
    Serial.println( "In Atmosphere sensor Data 2 loop");
      }
          Cancel =0;
    }  else if (Cancel == 1){
      Serial.print("Going back to main menu");
    } else {
    Serial.println( "Wrong Input");
    }

  } //------------------------------------------------------------------------------------------------------------------
}
Confirm =0; //
Cancel =0; 
}


/*

Serial.print("You entered: ");
Serial.println(Select);

// Button 1
if (Select == 1) 
{  
Serial.println();
Serial.println("Menu");
Serial.println("1 : Go Back to Main Menu"); 
Serial.println("2 : Increase speed of 2nd LED");
Serial.println("3 : Decrease speed of 2nd LED");
Serial.println("4 : Fade 3rd LED");
Serial.println();
Serial.println("Please input your Option 1-4");

toggle_memory = !toggle_memory;

while( Exit == 0){
  Serial.flush();
  while( Serial.available() == 0){
    if (toggle_memory) {
      digitalWrite(11, HIGH);
      delay(300);
    } else {
      digitalWrite(11,LOW);
      delay(300);
    }
  }
  while (Serial.available() >0 )
  {
    Exit = Serial.read() - '0';
  }

  if (Exit == 1){
  Serial.println(" Going back to main menu");
  Select_2 = 0;
  }else if (Exit == 2){
Select_2 = 2;
  } else if (Exit == 3){
Select_2 = 3;
  } else if (Exit == 4){
Select_2 = 4;
  } else {
  Serial.println(" Wrong input");
  Exit = 0;
  }
  }
}
delay(1000);

}


// Initialise toggle variables 
static byte toggle_memory=0;
static int LED_on = 0;
static int Delay = 500;
static int Fade= 0;
static int Select_2 = 0;
int Exit = 0 ;


if (Select_2 == 0){
tft.println("  Main Menu");
tft.println(" 1 : BME680 Sensor");
tft.println(" 2 : BH1750FVI Sensor");
tft.println();
tft.println(" Please input your Option 1-2");
Select =0;
Serial.flush(); // not needed


while( Serial.available() == 0){  
}

  while (Serial.available() > 0) {
    // wait for a user input
    Select = Serial.read() - '0';
  }
 //   Select = Serial.read();
  //  Serial.println(Select);
}
else
Select = Select_2;


Serial.print("You entered: ");
Serial.println(Select);

// Button 1
if (Select == 1) 
{  
Serial.println();
Serial.println("Menu");
Serial.println("1 : Go Back to Main Menu"); 
Serial.println("2 : Increase speed of 2nd LED");
Serial.println("3 : Decrease speed of 2nd LED");
Serial.println("4 : Fade 3rd LED");
Serial.println();
Serial.println("Please input your Option 1-4");

toggle_memory = !toggle_memory;

while( Exit == 0){
  Serial.flush();
  while( Serial.available() == 0){
    if (toggle_memory) {
      digitalWrite(11, HIGH);
      delay(300);
    } else {
      digitalWrite(11,LOW);
      delay(300);
    }
  }
  while (Serial.available() >0 )
  {
    Exit = Serial.read() - '0';
  }

  if (Exit == 1){
  Serial.println(" Going back to main menu");
  Select_2 = 0;
  }else if (Exit == 2){
Select_2 = 2;
  } else if (Exit == 3){
Select_2 = 3;
  } else if (Exit == 4){
Select_2 = 4;
  } else {
  Serial.println(" Wrong input");
  Exit = 0;
  }
  }
}

 // Button 2
else if ( Select == 2 ){

Delay = Delay - 50;
Serial.println();
Serial.println("Menu");
Serial.println("1 : Go Back to Main Menu");
Serial.println("2 : Toggle Top LED");
Serial.println("3 : Decrease speed of 2nd LED");
Serial.println("4 : Fade 3rd LED");
Serial.println("5 : Increase speed again");
Serial.println();
Serial.println("Please input your Option 1-5");

while( Exit == 0){
  Serial.flush();
  while( Serial.available() == 0){
      LED_on = !LED_on;
    if (LED_on == 0 )
    digitalWrite(10,LOW);
    else
    digitalWrite(10,HIGH);
    delay(Delay);
    }
     
    while (Serial.available() >0 )
  {
    Exit = Serial.read() - '0';
  }

  if (Exit == 1){
  Serial.println(" Going back to main menu");
  Select_2 = 0;
  }else if (Exit == 2){
Select_2 = 1;
  } else if (Exit == 3){
Select_2 = 3;
  } else if (Exit == 4){
Select_2 = 4;
  } else if (Exit == 5){
    Delay = Delay - 50;
    Exit= 0;
  }else {
  Serial.println(" Wrong input");
  Exit = 0;
  }
}
}
    // Button 3
else if ( Select == 3 ){

Delay = Delay + 50;
Serial.println();
Serial.println("Menu");
Serial.println("1 : Go Back to Main Menu");
Serial.println("2 : Toggle Top LED");
Serial.println("3 : Increase speed of 2nd LED");
Serial.println("4 : Fade 3rd LED");
Serial.println("5 : Decrease speed again");
Serial.println();
Serial.println("Please input your Option 1-5");

while( Exit == 0){
Serial.flush();
  while( Serial.available() == 0){
      LED_on = !LED_on;
    if (LED_on == 0 )
    digitalWrite(10,LOW);
    else
    digitalWrite(10,HIGH);
    delay(Delay);
    }
    while (Serial.available() >0 )
  {
    Exit = Serial.read() - '0';
  }

  if (Exit == 1){
  Serial.println(" Going back to main menu");
  Select_2 = 0;
  }else if (Exit == 2){
Select_2 = 1;
  } else if (Exit == 3){
Select_2 = 2;
  } else if (Exit == 4){
Select_2 = 4;
  } else if (Exit == 5){
    Delay = Delay + 50;
    Exit= 0;
  }else {
  Serial.println(" Wrong input");
  Exit = 0;
  }

/* if (Exit == 2){
  Serial.println(" Going back to main menu");
  Select_2=0;
  }
  else if( Exit == 1) {
    Delay = Delay + 50;
    Exit= 0;
  }else  {
  Serial.println(" Wrong input");
  Exit = 0;
  }
*/


/*
}
}
    // Button 4
    else if ( Select == 4 ){
Serial.println();
Serial.println("Menu");
Serial.println("1 : Go Back to Main Menu");
Serial.println("2 : Toggle Top LED");
Serial.println("3 : Increase speed of 2nd LED");
Serial.println("4 : Decrease speed of 2nd LED");
Serial.println();
Serial.println("Please input your Option 1-4");

while( Exit == 0){
       
    while( Serial.available() == 0){    
 //     Serial.flush();   
        Fade = !Fade;
        // Here we will fade the LED from 0 to maximum, 255
        if (Fade == 0)
        {
          for (int i = 0; i < 255; i++){
            analogWrite(9, i);
            delay(2);
          }}else if (Fade == 1) {
        // Fade the LED from maximum to 0
          for (int i = 254; i >= 0; i--){
            analogWrite(9, i);
            delay(2);
          }
        } else
        Serial.println( " Wrong Input detected");
    }

      while (Serial.available() >0 )
      {
        Exit = Serial.read() - '0';
      }

  if (Exit == 1){
  Serial.println(" Going back to main menu");
  Select_2 = 0;
  }else if (Exit == 2){
Select_2 = 1;
  } else if (Exit == 3){
Select_2 = 2;
  } else if (Exit == 4){
Select_2 = 3;
  }else {
  Serial.println(" Wrong input");
  Exit = 0;
  }
  
    }
  }
  Serial.println();
}


*/

void Menu1(){
Confirm = 1;
}

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

void Menu4(){
Cancel = 1;
}
