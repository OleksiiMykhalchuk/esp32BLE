#ifndef TEMP_SENSORE_H_
#define TEMP_SENSORE_H_

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

#endif /* TEMP_SENSORE_H_ */