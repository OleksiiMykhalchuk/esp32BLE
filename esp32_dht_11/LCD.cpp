#include "LCD.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void initLCD() {
  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("LCD Initialized!");
}

void printTemp(float temp, uint8_t humid, bool isOn, float setPoint) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("Temp: %.2f", temp);
  lcd.setCursor(0, 1);
  lcd.printf("Humid: %d", humid);
  lcd.setCursor(13, 1);
  if (isOn) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }
  lcd.setCursor(14, 0);
  lcd.print(setPoint);
}

void checkLCDI2CAddress() {
  Wire.begin();
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission () == 0)
      {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      
      delay (1);  // maybe unneeded?
      } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  // Serial.print (count, DEC);
  Serial.println (" device(s).");
}