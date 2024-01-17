#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>

#define PERIPHERAL_NAME "ESP32 BLE"
#define SERVICE_UUID "da30e919-38b1-469e-9081-da9f59c04c34"
#define CHARACTERISTIC_UUID "f8abccc0-488f-4747-8dd2-808a4c70bfc3"
#define CHARACTERISTIC_SETPOINT_UUID "04b250d7-c16b-4905-bbb3-65b597685dae"
#define CHARACTERISTIC_OUTPUT_UUID "ad0d60a7-6770-4383-a879-5cd77d75ec26"

void initBLE(float setPoint);
float sendData(float temp);

#endif /* BLUETOOTH_H_ */