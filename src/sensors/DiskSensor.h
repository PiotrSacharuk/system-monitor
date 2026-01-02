#ifndef DISK_SENSOR_H
#define DISK_SENSOR_H

#include "Sensor.h"

class DiskSensor : public Sensor {
  public:
    DiskSensor(double alertThreshold);
    void fetchData() override;
};

#endif