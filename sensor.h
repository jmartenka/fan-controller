#ifndef __INCLUDED_SENSOR__
#define __INCLUDED_SENSOR__

#include <Arduino.h>

#define SENSOR_ANALOG 1
#define SENSOR_DHT22 2


struct SensorDef {
  char* name;
  uint8_t pin;
  double maxTemp;
  double sensorFailTemp;
  uint8_t type;
};


class Sensor {
  public:
    Sensor() {}
    Sensor(const SensorDef* def);

    double rawTemp() const;
    double smoothedTemp() const;

    void measureTemp();
    void resetTemp();

    const SensorDef* def;

  private:
    double temp = 0.0;
    uint16_t counter = 0;

};

#endif
