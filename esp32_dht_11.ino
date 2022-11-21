#include <DHTesp.h>

DHTesp dht;

#define TEMP_PIN 13
uint32_t lastTime = 0;
uint32_t timerDelay = 5000;


void setup() {
  Serial.begin(9600);
  dht.setup(TEMP_PIN, DHTesp::DHT11);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    Serial.println("Temperatura = ");
    Serial.print(dht.getTemperature());
    Serial.println("\n");
    lastTime = millis();
  }

}
