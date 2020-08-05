#include <LiquidCrystal.h> // Including Liquid Crystal Library
#include <Keypad.h> // Including Matrix Keypad Library
#include <EEPROM.h> //Including EEPROM Library for Saving Variables Offline


// Setting Up Matrix KeyPad

const byte Rows = 4;
const byte Cols = 4;
char keys[Rows][Cols] = {
  {'C','1','2','3'},
  {'F','4','5','6'},
  {'D','7','8','9'},
  {'P','Z','0','E'}
};

byte rowPins[Rows] = {1,2,3,4}; //connect the row pinouts of the keypad
byte colPins[Cols] = {5,6,7,8}; //connect the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,Rows,Cols);

// Setting Up Matrix Keypad Completed

// Setting Up Liquid Crystal 

LiquidCrystal lcd(9,10,11,12,13,A0); // Pins to lcd, 9-RS PIN, 10 - Enable Pin, 11- Register D4, 12- Register D5, 13 -Register D6, A0 - Regsiter D7

// Seting Up Liquid Crystal Completed

// Global Variables
byte op_mode = 1; // There will be two methods of detecting Leak, depending of location/position at which the PT is installed

int threshold_pressure = EEPROM.read(0); // Setting Threshold to Last Stored Threshold

boolean interupt = true; // For accessing Settings
//int counters = 0; // For Reffering Menu/SubMenus in Settings

char password[]= "01024";

char option[][15] =
{
  {"Calibration"},
  {"Parameters"}
  //{"Parameters"},
  //{"Counts"},
  //{"Logs"}
};


int getpassword()
{
  int r= 3; // value to be returned
  int counter = 0;
  char getpassword[16];
  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0,1); // Setting cursor to 1 Column and 2 Row
  lcd.cursor();
  while(r == 3)
  {
    lcd.setCursor(counter,1);
    char text = keypad.waitForKey();
    if (text =='C')
    {
      //lcd.setCursor(0,0);
      //lcd.print("Cancelled");
      //delay(500);
      //lcd.clear();
      r = 2;
      lcd.noCursor();
      break;
    }
    else
    {
      if (text == "1" || text == "2" || text == "3" || text == "4" || text == "5" || text == "6" || text == "7" || text == "8" || text == "9")
      {
        getpassword[counter]=text;
        lcd.setCursor(counter,1);
        lcd.print("*");
        counter++;
        
      }
      else
      {
        if (text == "E")
        {
          for (int i=0; i<counter; i++)
          {
            if (getpassword[i]==password[i])
            {
              if (i == counter -1) {r =0;lcd.noCursor();}
            }
            else { r = 1; lcd.noCursor();break;}
          }
        }
      }
      
    }
    
  }
  return r;
}


void printfull(char text){
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print(text);
  delay(500);
}


int print_check()
{
  here:
  int r = getpassword();
  switch (r){
    case 1: // r =1 means Wrong Password has been inserted
      printfull("Wrong Password");
      goto here;
      return r;
      break;

    case 2:   // Cancelled by User
      printfull("Cancelled");
      return r;
      break;

    case 3:   // While Loop didn't run
      printfull("Error");
      return r;
      break;

    case 0:  // Successful Message
      printfull("Successful");
      return r;
      delay(200);
      break;
      
  }
 }
void operation(int i)
  {
    //lcd.clear();
    //lcd.print("Auto Salvaging");
  if (i == 1)
    { // Pressure Drop Detection Mode Active
    if (analogRead(A1)<threshold_pressure)
      {
      lcd.clear();
      lcd.print("Batch Passed");
      }
    else 
      {
      lcd.clear();
      lcd.print("Batch Failed");
      }
    }
  else
    {
    if (analogRead(A1)>threshold_pressure)
      {
      lcd.clear();
      lcd.print("Batch Passed");
      }
    else 
      {
      lcd.clear();
      lcd.print("Batch Failed");
      }     
    }
  }

 void setting(){
  int r = print_check();
  if (r==2){ goto here;}
  int counter = 0;
  char text;
  if (r==0){
    if (counter <0) { counter = counter +2;}
    if (counter >1) { counter = counter -2;}
    printfull(option[counter]);
    text = keypad.waitForKey();
    switch (text){
      case '4':
        counter--;
        break;
      case '6':
        counter++;
        break;
      case 'E':
        counter = counter;
        
    }
  }
  here:{}
 }
  
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  Serial.begin(9600);

  // setting up inputs of Arduino
  //pinMode(A0,OUTPUT); Already Taken into account during LCD Object
  pinMode(A1, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  char text = keypad.getKey();
  if (text == 'C')
    {
    interupt = true;  // Setting interupt True if any key is pressed
    }
  if (interupt = false)
    {
    operation(op_mode);
    }
  else
    {
    setting();
    }
}
