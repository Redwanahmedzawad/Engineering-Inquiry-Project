int cmstr;
char a='0';
int dr=0;
float temp=0,humid=0,gas=0,press=0,light=0;
void setup() {
  //Arduino IDE Serial Monitor
  Serial.begin(9600);
  Serial.println("Starting");

  //SDI-12
  Serial1.begin(1200, SERIAL_7E1);      //SDI-12 UART, configures serial port for 7 data bits, even parity, and 1 stop bit
  pinMode(7, OUTPUT);                   //DIRO Pin

  //DIRO Pin LOW to Send to SDI-12
  digitalWrite(7, LOW); 
  Serial1.println("HelloWorld");
  delay(100);

  //HIGH to Receive from SDI-12
  digitalWrite(7, HIGH); 
}

void loop(){
  //Receive SDI-12 over UART and then print to Serial Monitor
  char c = 'na';
  

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
        for (int i=0; i<zc; i++){
          msg = msg + "0";
        }
        msg = msg+"3"+"5";
          sdiout(msg);
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
            String msg = String(a)+ String("+") + String(temp) + String("+") + String(press) + String("+") + String(humid) + String("+") + String(gas);
            sdiout(msg);
            Serial.println(msg);
          }
        }
        else if(c == '2')
        {
          String msg = String(a)+String(light);
          sdiout(msg);
          Serial.println(msg);
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
    
    // Delay to remove noise
  delay(100);
}

}

void senval(int t)
{
  int ltime = millis();
  dr = (ltime -t)/1000;
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