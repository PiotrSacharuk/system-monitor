#ifndef CPU_SENSOR_H
#define CPU_SENSOR_H

#include "sensor.h"

class CpuSensor : public Sensor {
  private:
    int cores;
    unsigned long long prev_user, prev_nice, prev_system, prev_idle, prev_total;

  public:
    CpuSensor(int cpu_cores, double alert_threshold);
    void fetch_data() override;
};

#endif