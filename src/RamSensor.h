#ifndef RAM_SENSOR_H
#define RAM_SENSOR_H

#include "Sensor.h"

class RamSensor : public Sensor {
private:
    static int ramCycle;

public:
    RamSensor(double alertThreshold);
    void fetchData() override;
};

#endif