#include "TempSensore.h"

  TemperatureManager::TemperatureManager() {
    dht.setup(TEMP_PIN, DHTesp::DHT11);
  }

  Sensor TemperatureManager::getData() {
    Sensor sensor;
    data = dht.getTempAndHumidity();
    sensor.temp = data.temperature;
    sensor.humid = data.humidity;
    return sensor;
  }