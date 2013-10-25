int read_LCD_buttons()
{ 
adc_key_in = debounce(adc_key_in); // read the value from the sensor
if (adc_key_in > VNONE) return btnNONE; // We make this the 1st option for speed reasons since
if (adc_key_in < V1) return btnRIGHT;
if (adc_key_in < V2) return btnUP;
if (adc_key_in < V3) return btnDOWN;
if (adc_key_in < V4) return btnLEFT;
if (adc_key_in < V5) return btnSELECT;
return btnNONE; // when all others fail, return this...
}

// Improved reading of button state
int debounce(int last)
{
  int current = analogRead(btnPin); // Current button state
  if (last != current) // If a change happened
  {
    delay(10); // wait for bounce to settle
    current = analogRead(btnPin);
  }
  return current;
}
// Button down? wait for it to be released before continuing, unless user holds more than t_press
void waitforrelease(int t){
  cur_but = read_LCD_buttons();
  int t_press = millis();
  while (cur_but != btnNONE){
    cur_but = read_LCD_buttons();
    int cur_t = millis();
    if(cur_t > (t_press + t)) {
      if (t_wait > 100) t_wait = t_wait - 100;
      break;
    }
  }
}

// increment a variable via LCD buttons, within some limits
double increment_var(double out, double l_lim, double r_lim)
{
      lcd_key = read_LCD_buttons();
      //Serial.println(lcd_key);
      if (lcd_key == btnNONE) {
        t_wait = 600;
        return out;
      }
      waitforrelease(t_wait);
      switch (lcd_key){
        case btnUP:
        {
          if (out < r_lim) out++;
          break;
        }
        case btnDOWN:
        {
          if (out > l_lim) out--;
          break;
        }
        case btnRIGHT:
        {
          lcd.clear();
          select++;
          break;
        }
         case btnLEFT:
        {
          lcd.clear();
          select--;
          break;
        }
      }
      return out;
}

void stateswitch(void){
      lcd_key = read_LCD_buttons();
      //Serial.println(lcd_key);
      if (lcd_key == btnNONE) {
        t_wait = 600;
        return;
      }
      waitforrelease(t_wait);
      switch (lcd_key){
        case btnRIGHT:
        {
          lcd.clear();
          select++;
          break;
        }
         case btnLEFT:
        {
          lcd.clear();
          select--;
          break;
        }
      }
      return;
}

// MegunoLink Plotting
void sendPlotData(String seriesName, double data)
{
  Serial.print("{");
  Serial.print(seriesName);
  Serial.print(",T,");
  Serial.print(data);
  Serial.println("}");
}
