#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Define the 1-Wire bus and the EEPROM device
OneWire oneWire(8); // Connect to pin D2
DallasTemperature sensors(&oneWire);

// Define the LCD connections
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, EN, D4, D5, D6, D7

void setup() {
  Serial.begin(9600);
  //lcd.begin(16, 2); // Set the LCD dimensions (16x2)

  // Start communication with the 1-Wire devices
  sensors.begin();
}

void loop() {
  // Write your student ID to the EEPROM
  byte studentID[] = {'S', 'T', 'U', 'D', 'E', 'N', 'T', 'I', 'D'};
  EEPROMWrite(0, studentID, sizeof(studentID));

  // Read the student ID from EEPROM
  byte readStudentID[sizeof(studentID)];
  EEPROMRead(0, readStudentID, sizeof(readStudentID));

  // Display the student ID on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Student ID:");
  lcd.setCursor(0, 1);
  lcd.print((char*)readStudentID);

  delay(5000); // Display for 5 seconds
}

void EEPROMWrite(int address, byte data[], int dataSize) {
  for (int i = 0; i < dataSize; i++) {
    EEPROM.write(address + i, data[i]);
  }
  EEPROM.commit(); // Save changes to EEPROM
}

void EEPROMRead(int address, byte data[], int dataSize) {
  for (int i = 0; i < dataSize; i++) {
    data[i] = EEPROM.read(address + i);
  }
}
