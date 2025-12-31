#ifndef CPU_SENSOR_H
#define CPU_SENSOR_H

#include "Sensor.h"

class CpuSensor : public Sensor {
private:
    int cores;
    unsigned long long prevUser, prevNice, prevSystem, prevIdle, prevTotal;

public:
    CpuSensor(int cpuCores, double alertThreshold);
    void fetchData() override;
};

#endif