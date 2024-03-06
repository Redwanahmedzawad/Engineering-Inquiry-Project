#include <OneWire.h>
//#include <EEPROM.h>
// Define the one wire bus and the address of the DS28E07 chip
OneWire ds(2);  // One wire bus is on digital pin 2
byte addr[] = {0x2A, 0x0 , 0x0 , 0x0 , 0xD0 , 0x0 , 0x0 , 0x20}; //insert the Address of EEPROM value into here after you run the searchFunction

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  Serial.println("-------------------------------------------------------------------------------");

  Serial.println("Reading from EEPROM (FROM MEMORY)");       //Reading from eeprom after power-cycle
  String EEPROMString = readFromEEPROM(2);
  EEPROMString.remove(7,12);            //trim undefined characters
  Serial.println(EEPROMString);

  Serial.println("Getting Family code / Address of EEPROM:");
  searchFunction();

  // Write a test string to the EEPROM
  Serial.println("Writing to EEPROM"); 
  String testString = "NewLine ";
  writeToEEPROM(testString);

  // Read the string back from the EEPROM and print it to the serial monitor
  Serial.println("Reading from EEPROM");
  String readString = readFromEEPROM(testString.length());
  //readString.remove(7,12);
  
  Serial.println(readString);
}

void loop() {

}

void searchFunction() {
  byte i;
  byte addr[8];

  if (!ds.search(addr)) {
    ds.reset_search();
    delay(1000);
  }

  for (i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\n");
}

// Function to write a string to the DS28E07 EEPROM
void writeToEEPROM(String data) {
  // Convert the input string to a char array
  char dataChars[30];
  data.toCharArray(dataChars, 30);

  // Write the data to the EEPROM
  ds.reset(); //reset device
  ds.select(addr);//select address of device to talk to
  ds.write(0x0F,1);   // Write ScratchPad       Configuration/mode bytes can be in datasheet
  ds.write(0x01,1);   //TA1 addresses           TA (Target Address) can also be found in the datasheet
  ds.write(0x09,1);   //TA2 addresses
  for ( int i = 0; i < data.length(); i++) {
    ds.write(dataChars[i],1);
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x0F, 1);  // Copy ScratchPad
}

// Function to read a string from the DS28E07 EEPROM
String readFromEEPROM(int length) {
  byte dataBytes[30];
  String str="";
  ds.reset();
  ds.select(addr);
  ds.write(0xAA);  // Read Scratchpad
  for (int i = 0; i < length+4; i++) {
    dataBytes[i] = ds.read();
   // str+= (char*)dataBytes[i];
   // Serial.println(dataBytes[i]);
  }
  return String((char*)dataBytes).substring(3);
  //return str;
}