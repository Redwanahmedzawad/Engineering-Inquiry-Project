

const char error_message[] PROGMEM = {"Could not connect to serial device"};
const unsigned long baud_rates[] PROGMEM = {110, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};

void setup() {
    Serial.begin(9600);
  
    // print out each character from the error message, one char at a time. 
    for (byte i = 0; i < strlen(error_message); i++) {
        // we need to recall each char one byte at a time, and 
        // use the i as an incrementer to get each new byte in a sequence
        char c = pgm_read_byte_near(error_message + i);
        Serial.print(c);
    }
    Serial.println();

    Serial.print("Baud rate set at: ");
    Serial.println(pgm_read_dword_near(baud_rates + 5)); 
    // 115200. Notice how we don't use the array index [] for this
}


void loop() {
  // put your main code here, to run repeatedly:

}
