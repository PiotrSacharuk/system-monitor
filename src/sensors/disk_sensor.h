#ifndef DISK_SENSOR_H
#define DISK_SENSOR_H

#include "sensor.h"

class DiskSensor : public Sensor {
  public:
    DiskSensor(double alert_threshold);
    void fetch_data() override;
};

#endif