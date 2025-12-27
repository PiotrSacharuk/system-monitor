#ifndef CPU_SENSOR_H
#define CPU_SENSOR_H

#include "Sensor.h"

class CpuSensor: public Sensor {
private:
    int cores;
    static int globalCycle;

public:
    CpuSensor(int cpuCores, double alertThreshold);
    void fetchData() override;
};

#endif