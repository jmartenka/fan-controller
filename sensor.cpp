#include "sensor.h"
#include <DHT.h>

DHT dht(2, DHT22);


Sensor::Sensor(const SensorDef* def): def(def) {
  this->def = def;

  if (this->def->type == SENSOR_ANALOG)
  {
    pinMode(def->pin, INPUT);
  }

  if (this->def->type == SENSOR_DHT22)
  {
    dht.begin();
  }
}


double Sensor::rawTemp() const {
  if (this->def->type == SENSOR_ANALOG)
  {
    double tempLog = log(10000.0 * (1024.0 / analogRead(def->pin) - 1));
    return 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempLog * tempLog )) * tempLog ) - 273.15;
  }

  if (this->def->type == SENSOR_DHT22)
  {
    float t = dht.readTemperature();

    if (isnan(t))
    {
      Serial.println("Failed to read from DHT");
    }
    else
    {
      return (t);
    }

    return this->def->sensorFailTemp;
  }
}


double Sensor::smoothedTemp() const {
  return temp;
}

void Sensor::measureTemp() {
  temp = (temp * (double)counter + rawTemp()) / ((double)counter + 1.0);
  counter++;
}

void Sensor::resetTemp() {
  counter = 0;
}
