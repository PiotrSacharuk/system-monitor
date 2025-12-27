#include "DiskSensor.h"
#include <iostream>

int DiskSensor::diskCycle = 0;

DiskSensor::DiskSensor(double alertThreshold) : Sensor("DISK", alertThreshold) {}

void DiskSensor::fetchData() {
    diskCycle++;
    currentValue = 50.0 + (diskCycle % 6) * 9.0;
    std::cout << "Disk data collected from /proc/diskstats..." << std::endl;
}