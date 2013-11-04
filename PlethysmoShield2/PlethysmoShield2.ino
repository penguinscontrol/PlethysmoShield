#include <LiquidCrystal.h>
#include <Statistics.h>

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
// 
int ledPin = 3;
int drive = 255;
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
int t_now = millis();
//Variables
boolean isbeating = 0;
boolean isflashing = 0;

double t_flash = 0;
int last_updated;
int curVal = analogRead(plePin); // current sample
int lastVal = 0;
int dcurVal = 0;

Statistics dsig_sta(WL); // statistics on derivative of signal
Statistics sig_sta(WL);

int beat_times[WL/10]; // times of beats
int t_ind = 0;

double thresh = 200; // Threshold for beat detection

int setpoint = 768;
int err = 0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2); // start the LCD
  lcd.createChar(0, heart);

  lcd.setCursor(0,0);
  lcd.print("Initializing...");

  for (int a=0; a < WL; a++){
    lastVal = curVal;
    curVal = analogRead(plePin);
    dcurVal = curVal-lastVal;
    dsig_sta.addData(double(dcurVal));
    sig_sta.addData(double(curVal));
    delay(TS);
  }

  thresh = dsig_sta.mean()+1.5*dsig_sta.stdDeviation();
  lcd.clear();
}

void loop()
{
  delay(TS); 
  switch(select) {
  case 0:
    {
      //analogWrite(ledPin,drive);
      analogWrite(ledPin,255);
      update_sig();
      // Print Message
      lcd.setCursor(0,0);
      lcd.print("HRMon");

      // update signal
      lastVal = curVal;
      curVal = analogRead(plePin);
      dcurVal = curVal-lastVal;
      dsig_sta.addData(double(dcurVal));
      sig_sta.addData(double(curVal));
      
      checkForBeats();
      if (millis()>last_updated+1000){
        updateHR();
      }

      sendPlotData("Signal",curVal);
      sendPlotData("dS/dt",dcurVal);
      sendPlotData("Threshold",thresh);

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
void update_sig(){
  err = double(setpoint) - sig_sta.mean() - 3*dsig_sta.stdDeviation();
  if (err > 10) {
    drive++;
  }
  else if (err < -10){
    drive--;
  }
}
void updateHR(){
  int beats = 0;
   
  thresh = dsig_sta.mean()+1.5*dsig_sta.stdDeviation();
  last_updated = millis(); 
  for (int a = 0; a<WL/10;a++){
    if (beat_times[a]>last_updated-5000){
      beats++;
    }
  }
  lcd.setCursor(3,1);
  lcd.print(beats*12);
  lcd.print(" BPM");
}

void checkForBeats(){

  isbeating = dcurVal > thresh; // did I just cross the threshold?
  if (isbeating && !isflashing && millis() > t_flash+200 ) {
    isflashing = 1;

    beat_times[t_ind] = millis();
    t_ind++;
    if (t_ind > WL/10-1){
      t_ind = 0;
    }

    t_flash = millis();
    lcd.setCursor(0,1);
    lcd.write(byte(0));
  }

  if (millis() > t_flash+200) {
    isflashing = 0;
    lcd.setCursor(0,1);
    lcd.print(" ");
  }

}


