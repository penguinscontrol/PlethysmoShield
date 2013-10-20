#include <LiquidCrystal.h>
#include <QueueArray.h>

//Define sampling parameters
#define TS 20 // Sampling rate of 50 Hz
#define WL 250 // Window length in samples: 5 seconds

//Define button voltage ranges
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5
#define V1 20
#define V2 119
#define V3 276
#define V4 430
#define V5 660
#define VNONE 1003

//Initialize LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Define input pins
int btnPin = 0;
int plePin = 1;

//Button presses
int lcd_key = 0; // what button is being pressed?
int adc_key_in = 0; // what voltage is being applied to button pin?
int cur_but = btnNONE;
int t_wait = 600; // how long until a pressed button registers again?

// ***** DEGREE SYMBOL FOR LCD *****
byte heart[8] = {
  B00000,
  B01010,
  B10101,
  B10001,
  B01010,
  B00100,
  B00000,
};

//Which module of the program to run?
int select = 0;

//Variables
boolean isbeating = 0;
double curVal = analogRead(plePin); // current sample
double dcurVal = 0;
double sig[WL]; // Last 5 seconds of signal
double dsig[WL-1]; // Derivative of signal

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2); // start the LCD
  lcd.createChar(0, heart);
  lcd.setCursor(0,0);
  lcd.print("Initializing...");

  for (int a=0; a < WL; a++){
    sig[a] = analogRead(plePin);
    if (a != 0){
      dsig[a-1] = sig[a] - sig[a-1];
    }
    delay(TS);
  }
  lcd.clear();
}

void loop()
{
  delay(TS); 
  switch(select) {
  case 0:
    {
      // Print Message
      lcd.setCursor(0,0);
      lcd.print("HRMon");


      // update signal
      curVal = double(analogRead(plePin));
      dcurVal = curVal-sig[WL-1];
      
      updatevar(dsig,WL-1,dcurVal);
      updatevar(sig, WL, curVal);

      isbeating = curVal > 600;

      if (isbeating) {
        lcd.setCursor(0,1);
        lcd.write(byte(0));
      }
      else {        
        lcd.setCursor(0,1);
        lcd.print(" ");
      }
      
      sendPlotData("Signal",sig[WL-1]);
      sendPlotData("dS/dt",dsig[WL-2]);

      stateswitch(); //check if should switch states
      break;
    }
  case 1:
    {
      lcd.setCursor(0,0);
      lcd.print("Phase Two");
      stateswitch(); //check if should switch states
      break;
    }
  case 3:
    {
      break;
    }
  }
}

void updatevar(double *arr,int len,int newVal){
  for (int a = 0; a < len-1; a++){
    arr[a] = arr[a+1];
  }
  arr[len-1] = double(newVal);
}

