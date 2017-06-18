// Counter.ino - Example from the SAA1064 library for Arduino
// http://photonicsguy.ca/projects/electronics/7segdisplay

#include <Wire.h>	// This library is required to be included as well
#include "SAA1064.h"

SAA1064 Display1 = SAA1064(0);
SAA1064 Display2 = SAA1064(1);
SAA1064 Display3 = SAA1064(2);
SAA1064 Display4 = SAA1064(3);
int temp=0;
int temp1;
int temp2;
int temp3;
void setup() {
  Display1.begin();
  Display1.brightness(5);
  Display1.dash();  // Display dashes

  Display2.begin();
  Display2.brightness(5);
  //Display2.dash();  // Display 

  Display3.begin();
  Display3.brightness(5);
  //Display3.dash();  // Display dashes

  Display4.begin();
  Display4.brightness(5);
  Display4.dash();  // Display dashes
  delay(4000);
}

void loop() {
  delay(100);  
 /* for(int i=100;i<=600;i+=40) {
    Display4.print(i);
    delay(20);
    if(i>=0){
      delay(400);
    }
  }*/
    for(int i=0;i<=10;i++)
    {
      temp = temp + 9;
      temp1 = temp/10;
      temp1 *=100;
      temp2 = temp%10;
      temp3 = temp1+temp2;
     // Display1.begin();
      Display1.print(temp3);
      /*Display2.begin();
      Display2.print(temp3);
      Display3.begin();
      Display3.print(temp3);
      Display4.begin();
      Display4.print(temp3);*/
      
      delay(1000);
    
    }
    temp=0;
  
}



