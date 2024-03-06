#include "lib.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(matrix[1][0][0]);
  for(int i = 5; i < 14; i++){
    pinMode(i, OUTPUT);
   digitalWrite(i, LOW); 
  }
  for(int i = 10; i < 14; i++){
   digitalWrite(i, HIGH); 
  }
  delay(1000);


}
//13-10 - left to right 
//9 - 5 + top to bottom
int ii = 0;
int display_for = 1600; //1.6s
void loop() {


  /*  
    Character A Structure 5X4:
            0 1 2 3 - column
    row  0 |0,1,1,0
         1 |1,0,0,1
         2 |1,1,1,1
         3 |1,0,0,1
         4 |1,0,0,1
    Hint: in order to display a character on LED matrix, you need to create a loop through each colume of the character (colume 0 - 1) 
          in side the loop, we need to turn on/off LED of each row in the colume- in colume 0, the Led on row 0 must be on, the led from row 1 to 4 must be on.
          after turn on/off different LEd in the colume, we need to turn off (clear) all the Leds in that colume.
          next loop, we go to next colume, and we will do the same thing

    for loop - loop thourgh each colume of that character - called loopA
      turn on/off the row 0 Led
      turn on/off the row 1 Led
      turn on/off the row 2 Led
      turn on/off the row 3 Led
      turn on/off the row 4 Led
      turn off row 0/1/2/3/4 Led - Can only turn off the Led if that Led is turned on before 
    end of loop
    to display the character nice and clear, we will keep display the same character for several of time 
    NOTE: do not use any delay() in loopA.
    // perform loopA 1600 time = display A very fast for 1600s 
    for loop - loop for 1600 
      loopA
    end of loop 

    After you can display a character on Led matrix - to make it scroll, you need to control the displayed column of the character 

  */
  print_Matrix(1);
  delay(2000);
  print_Matrix(2);
  delay(2000);


}
void print_Matrix(int x)
{
  for(int k = 0; k <3000;k++){
    for (int i=0;i<5;i++)
        {
          for (int j =0;j<4;j++)
            {
              if (matrix[x][i][j]==1)
                {
                  display_LED_at(j,i);

                }
      
            }
        }
    }
  
}

void display_LED_at(int x, int y){
      digitalWrite((13 - x ), LOW); //row
      digitalWrite((9 - y ), HIGH); //clm
      digitalWrite((13 - x ), HIGH);
      digitalWrite((9 - y ), LOW);
      
}
