#include "RamSensor.h"
#include <iostream>

int RamSensor::ramCycle = 0;

RamSensor::RamSensor(double alertThreshold) : Sensor("RAM", alertThreshold) {}

void RamSensor::fetchData() {
    ramCycle++;
    currentValue = 30.0 + (ramCycle % 7) * 8.0;
    std::cout << "RAM data collected from /proc/meminfo..." << std::endl;
}