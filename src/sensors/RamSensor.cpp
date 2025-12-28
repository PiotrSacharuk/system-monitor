#include "RamSensor.h"
#include <iostream>
#include <fstream>

RamSensor::RamSensor(double alertThreshold) : Sensor("RAM", alertThreshold) {}

void RamSensor::fetchData() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    unsigned long long total = 0, available = 0;

    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0) {
            sscanf(line.c_str(), "MemTotal: %llu kB", &total);
        } else if (line.find("MemAvailable:") == 0) {
            sscanf(line.c_str(), "MemAvailable: %llu kB", &available);
        }
    }

    if (total > 0) {
        currentValue = 100.0 * (1.0 - (double)available / total);
    }
    std::cout << "RAM data collected from /proc/meminfo (" << (int)currentValue << "%)" << std::endl;
}