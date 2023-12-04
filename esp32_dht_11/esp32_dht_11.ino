
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "LCD.h"
#include "TempSensore.h"

TemperatureManager sensor;
Sensor model;
// DHTesp dht;

// #define TEMP_PIN 13
uint32_t lastTime = 0;
uint32_t timerDelay = 5000;
double temp;
double const offset = 6.0;
float setPoint = 20.0;

#define PERIPHERAL_NAME "ESP32 BLE"
#define SERVICE_UUID "da30e919-38b1-469e-9081-da9f59c04c34"
#define CHARACTERISTIC_UUID "f8abccc0-488f-4747-8dd2-808a4c70bfc3"
#define CHARACTERISTIC_INPUT_UUID "04b250d7-c16b-4905-bbb3-65b597685dae"
#define CHARACTERISTIC_OUTPUT_UUID "ad0d60a7-6770-4383-a879-5cd77d75ec26"

static uint8_t outputData[1];

BLECharacteristic *pOutputChar;

class ServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    Serial.println("BLE Client connected");
  }
  void onDisconnect(BLEServer *pServer) {
    BLEDevice::startAdvertising();
    Serial.println("BLE Client Disconnected");
  }
};

class InputReceivedCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharWriteState) {
    uint8_t *inputValues = pCharWriteState->getData();
    switch(inputValues[2]) {
      case 0x00: // add
      outputData[0] = inputValues[0] + inputValues[1];
      break;
      case 0x01: // subtract
      outputData[0] = inputValues[0] - inputValues[1];
      break;
      default: // multiply
      outputData[0] = inputValues[0] * inputValues[1];
      break;
    }
    pOutputChar->setValue((uint8_t *)outputData, 1);
    pOutputChar->notify();
    Serial.println("input received ");
    Serial.print(*inputValues);
    Serial.println();
  }
};

void setup() {
  Serial.begin(9600);
  
  sensor = TemperatureManager();

  BLEDevice::init(PERIPHERAL_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pInputChar = pService->createCharacteristic(
    CHARACTERISTIC_UUID, 
    BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_WRITE);

  pOutputChar = pService->createCharacteristic(CHARACTERISTIC_OUTPUT_UUID,
  BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  pInputChar->setCallbacks(new InputReceivedCallbacks());
  pServer->setCallbacks(new ServerCallbacks());

  outputData[0] = 0x00;
  pOutputChar->setValue((uint8_t *)outputData, 1);

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();

  initLCD();
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
    outputData[0] = current;
    pOutputChar->setValue((uint8_t*)outputData, 1);
    pOutputChar->notify();
    
    lastTime = millis();

    bool isOn;
    if (current < setPoint) { 
      isOn = true;
    } else {
      isOn = false;
    }

    printTemp(current, (uint8_t)humid, isOn, setPoint);
  }
}
