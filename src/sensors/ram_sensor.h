#ifndef RAM_SENSOR_H
#define RAM_SENSOR_H

#include "sensor.h"

class RamSensor : public Sensor {
  public:
    RamSensor(double alert_threshold);
    void fetch_data() override;
};

#endif