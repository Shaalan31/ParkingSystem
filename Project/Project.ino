#include <Wire.h>
#include "SAA1064.h"
#include "Keypad.h"
#include <EEPROM.h>

#define INTERNAL 1.1

//Pin Configurations
//----------------------//
int smokePin = A1;
int tempPin = A0;
int fireLedR = 3;
int fireLedB = 5;
int fireLedG = 6;
int Pin51=11;
int Pin49=12;
int SDApin = 20;
int SCLpin = 21;
int lightSensor2 = 52;
int lightSensor1 = 53;

//Variables Required
//-------------------------//
int tempEnter=1;
int tempExit=1;
int gasReading;
int tempReading;
int count;
bool Opened;
int temp1;
int temp2;
int temp3;

//Password
String str="";
String pass="7796";
char key;

//Time Implementation
unsigned long current=0;

long previousMillisEntrance = 0;
long intervalEntrance = 1000;

long previousMillisExit = 0;
long intervalExit = 1000;

long previousMillisTemp = 0;
long intervalTemp = 10000;

long previousMillisGas = 0;
long intervalGas = 1000;

long intervalReset = 0000;
long previousMillisReset=0;


//SAA1064 Initialization
SAA1064 Display1 = SAA1064(0);

//Keypad Initialization
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte colPins[COLS] = {25 ,24, 23, 22}; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = {29, 28, 27, 26}; //connect to the row pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  // put your setup code here, to run once:
  analogReference(INTERNAL);

  //Initializing Serial
  Serial.begin(9600);

  //Get Values From EEPROM
  count = EEPROM.read(0);
  Opened = EEPROM.read(1);

  //Setting Up Light Sensors
  pinMode(lightSensor1,INPUT);
  pinMode(lightSensor2,INPUT);

  //Setting Up 5V Pins
  pinMode(Pin51,OUTPUT);
  digitalWrite(Pin51,HIGH);
  pinMode(Pin49,OUTPUT);
  digitalWrite(Pin49,HIGH);

  //Setting Up 7 Segments
  Display1.begin();
  Display1.brightness(5);
  Display1.dash();
}

void loop() 
{
  if(Opened)
  {
      //Calling Functions
      checkEntrance();
      checkExit();
      sevenSegment();
      gasSensor();
      tempSensor();
  
      key = keypad.getKey();
      if(key == '#')
      {
          if(checkPassword())
          {
            Opened = false;
            EEPROM.update(0,count);
            EEPROM.update(1,Opened);
          }
      }
  }
  else
  {
        gasSensor();
        tempSensor();
    
        key = keypad.getKey();
        if(key == '#')
        {
            if(checkPassword())
            {
              Opened=true;
              count = EEPROM.read(0);
              EEPROM.update(1,Opened);
            }
        }
  }
   
  EEPROM_Update();
}

void checkEntrance()
{
  if(!(tempEnter == 0 && digitalRead(lightSensor1) == 0))
  {
      tempEnter = digitalRead(lightSensor1);
      current = millis();
  
 /*     Serial.print("Enter : ");
      Serial.print(tempEnter);
      Serial.print("\n");*/
    
      if(tempEnter == 0)
       {
          if(count!=0 && ( current - previousMillisEntrance > intervalEntrance))
          {
        
            previousMillisEntrance = current;
            count--;
          }
       }
  }
  return;
}

void checkExit()
{
  if(!(tempExit == 0 && digitalRead(lightSensor2) == 0))
  {
      tempExit = digitalRead(lightSensor2);
      current = millis();
      
       if(tempExit == 0)
       {
            if(count!=99 && (current - previousMillisExit > intervalExit))
            {
                Serial.print("Exit : ");
                Serial.print(tempExit);
                Serial.print("\n");
                previousMillisExit = current;
                count++;
            }
       }
  }
  return;
}

void sevenSegment()
{
      temp1 = count/10;
      temp1 *=100;
      temp2 = count%10;
      temp3 = temp1+temp2;
      Display1.print(temp3);
      return;
}


void gasSensor()
{
    current = millis();
    if(current - previousMillisGas > intervalGas)
    {
        Serial.print("Gas Reading : ");
        Serial.print(gasReading);
        Serial.print("\n");
        previousMillisGas = current;
        gasReading = analogRead(smokePin);
    
        if(gasReading>450)
        {
          analogWrite(fireLedR, /*gasReading/*/100);
          analogWrite(fireLedG, 0 );
          analogWrite(fireLedB, 00 /*gasReading%10*/);
          //Opened = false;
        }
        else
        {
          analogWrite(fireLedR, /*gasReading/*/00);
          analogWrite(fireLedG, 50 /*(gasReading%100)/10*/);
          analogWrite(fireLedB, 0 /*gasReading%10*/);
        //  Opened = true;
        }
    }
    return;
}

void tempSensor()
{
  current = millis();
  if(current - previousMillisTemp > intervalTemp)
  { 
      previousMillisTemp = current;
      tempReading = analogRead(tempPin)/9.31;
      //tempReading = (5.0 * analogRead(tempPin) * 100.0) / 1024;
      Serial.print("Temp Reading :");
      Serial.print(tempReading);
      Serial.print("\n");
      if(tempReading > 800 )
      {
   //     digitalWrite(relayPin,HIGH);
      }
      else
      {
//        digitalWrite(relayPin,LOW);
      }
   }
    return;
}

bool checkPassword()
{
   key = keypad.getKey();
    
    while(key != '*')
    {
       String stringOne =  String(key);
       str.concat(stringOne);
       key = keypad.getKey();
    }
    
    if( str == pass)
    {
       Display1.dash(); 
       str="";
       return true;
    }
    else 
    {
       Display1.dash(); 
       str="";
       return false;
    }
}

void EEPROM_Update()
{
    current = millis();
    if((current - previousMillisReset > intervalReset) && Opened)
    {
        previousMillisReset = current;
        EEPROM.update(0,count);
    }
    return;
}
