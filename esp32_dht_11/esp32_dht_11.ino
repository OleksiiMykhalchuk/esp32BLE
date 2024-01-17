
#include "LCD.h"
#include "TempSensore.h"
#include "BluetoothManager.h"

#define RELAY_PIN 5
#define INTERNAL_LED_PIN 2

TemperatureManager sensor;
Sensor model;

uint32_t lastTime = 0;
uint32_t timerDelay = 5000;
double temp;
double const offset = 2.0;
float setPoint = 20.0;
bool isOn = false;

void setup() {
  Serial.begin(9600);
  
  sensor = TemperatureManager();

  initBLE(setPoint);

  initLCD();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(INTERNAL_LED_PIN, OUTPUT);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    model = sensor.getData();
    temp = model.temp;
    float humid = model.humid;
    float current = temp - offset;
    Serial.println("Temperatura = ");
    Serial.print(current);
    Serial.println("");
    setPoint = sendData(current);
    
    lastTime = millis();

    if (current < (setPoint - 0.3)) { 
      isOn = true;
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(INTERNAL_LED_PIN, HIGH);
    } else if (current > (setPoint + 0.3)) {
      isOn = false;
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(INTERNAL_LED_PIN, LOW);
    }

    printTemp(current, (uint8_t)humid, isOn, setPoint);
  }
}
