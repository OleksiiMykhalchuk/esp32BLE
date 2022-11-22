#include <DHTesp.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

DHTesp dht;

#define TEMP_PIN 13
uint32_t lastTime = 0;
uint32_t timerDelay = 5000;
double temp;
double offset = 2.0;

#define PERIPHERAL_NAME "ESP32 BLE"
#define SERVICE_UUID "da30e919-38b1-469e-9081-da9f59c04c34"
#define CHARACTERISTIC_UUID "f8abccc0-488f-4747-8dd2-808a4c70bfc3"

class InputReceivedCallbacks: public BLECharacteristicCallbacks {
  
};

void setup() {
  Serial.begin(9600);
  dht.setup(TEMP_PIN, DHTesp::DHT11);

  BLEDevice::init(PERIPHERAL_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pInputChar = pService->createCharacteristic(
    CHARACTERISTIC_UUID, 
    BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_WRITE);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    TempAndHumidity data = dht.getTempAndHumidity();
    temp = data.temperature;
    Serial.println("Temperatura = ");
    Serial.print(temp - offset);
    Serial.println("");
    
    lastTime = millis();
  }
}
