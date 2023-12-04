#ifndef LCD
#define LCD

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Arduino.h>

void initLCD();
void printTemp(float temp, uint8_t humid, bool isOn, float setPoint);
void checkLCDI2CAddress();

#endif