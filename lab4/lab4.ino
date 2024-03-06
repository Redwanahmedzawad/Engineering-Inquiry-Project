int opnum;
int ledon=0;
int tim = 200;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int opt[4]={1,2,3,4};
  pinMode(11, OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(6,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(10,HIGH);
  delay(tim);
  digitalWrite(10,LOW);
  delay(tim);
  if(Serial.available()){
    char ch=Serial.read();
    if(isDigit(ch))
    {
        opnum = (ch-'0');
    }
  }
  if(opnum == 1)
  {
    if(ledon==0){
          digitalWrite(11,HIGH);
          delay(500);
          ledon=1;
        }
        else
        {
          digitalWrite(11,LOW);
          delay(500);
          ledon = 0;
        }
        opnum=9;
  }
 
}
