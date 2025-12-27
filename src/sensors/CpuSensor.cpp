#include "CpuSensor.h"
#include <iostream>

int CpuSensor::globalCycle = 0;

CpuSensor::CpuSensor(int cpuCores, double alertThreshold)
    : Sensor("CPU", alertThreshold), cores(cpuCores) {}

void CpuSensor::fetchData() {
    globalCycle++;
    currentValue = 10.0 + (globalCycle % 8) * 10.0;
    std::cout << "CPU data collected from /proc/cpuinfo..." << std::endl;
}