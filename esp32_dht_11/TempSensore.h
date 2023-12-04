#ifndef TEMP_SENSORE
#define TEMP_SENSORE

#include <DHTesp.h>

#define TEMP_PIN 13

struct Sensor {
  float temp;
  float humid;
};

class TemperatureManager {

  private:
    DHTesp dht;
    TempAndHumidity data;

  public:
  TemperatureManager();

  Sensor getData();
};

#endif