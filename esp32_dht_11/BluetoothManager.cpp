#include "BluetoothManager.h"

static uint8_t outputData[1];
static uint8_t setpointData[1];

BLECharacteristic *pOutputChar;
BLECharacteristic *pSetpointChar;

float bleSetPoint;

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
    bleSetPoint = (float)*inputValues;
  }
};

void initBLE(float setPoint) {
  bleSetPoint = setPoint;
  BLEDevice::init(PERIPHERAL_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pInputChar = pService->createCharacteristic(
    CHARACTERISTIC_UUID, 
    BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_WRITE);

  pOutputChar = pService->createCharacteristic(CHARACTERISTIC_OUTPUT_UUID,
  BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  pSetpointChar = pService->createCharacteristic(CHARACTERISTIC_SETPOINT_UUID,
  BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  pInputChar->setCallbacks(new InputReceivedCallbacks());
  pServer->setCallbacks(new ServerCallbacks());

  outputData[0] = 0x00;
  pOutputChar->setValue((uint8_t *)outputData, 1);

  setpointData[0] = setPoint;
  pSetpointChar->setValue((uint8_t *)setpointData, 1);

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
}

float sendData(float temp) {
    outputData[0] = temp;
    pOutputChar->setValue((uint8_t*)outputData, 1);
    pOutputChar->notify();
    setpointData[0] = bleSetPoint;
    pSetpointChar->setValue((uint8_t *)setpointData, 1);
    pSetpointChar->notify();
    return bleSetPoint;
}