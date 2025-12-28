#include "DiskSensor.h"
#include <iostream>
#include <fstream>

DiskSensor::DiskSensor(double alertThreshold) : Sensor("DISK", alertThreshold) {}

void DiskSensor::fetchData() {
    std::ifstream diskStats("/proc/diskstats");
    std::string line;
    unsigned long long reads = 0, writes = 0;

    while (std::getline(diskStats, line)) {
        if (line.find("sda ") != std::string::npos) {
            unsigned major, minor, blocks;
            sscanf(line.c_str(), "%u %u %*s %llu %*u %*u %*u %*u %llu",
                &major, &minor, &reads, &writes);
            break;
        }
    }
    currentValue = std::min(95.0, 40.0 + (reads % 100) / 2.0);
    std::cout << "Disk data collected from /proc/diskstats (" << (int)currentValue << "%)" << std::endl;
}