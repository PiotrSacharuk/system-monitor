#include "disk_sensor.h"

#include <fstream>
#include <iostream>

DiskSensor::DiskSensor(double alert_threshold) : Sensor("DISK", alert_threshold) {}

void DiskSensor::fetch_data() {
    try {
        std::ifstream disk_stats("/proc/diskstats");
        std::string line;
        unsigned long long reads = 0, writes = 0;

        while (std::getline(disk_stats, line)) {
            if (line.find("sda ") != std::string::npos) {
                unsigned major, minor, blocks;
                sscanf(line.c_str(), "%u %u %*s %llu %*u %*u %*u %*u %llu", &major, &minor, &reads,
                       &writes);
                break;
            }
        }
        current_value = std::min(95.0, 40.0 + (reads % 100) / 2.0);
        std::cout << "Disk data collected from /proc/diskstats (" << (int)current_value << "%)"
                  << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Disk Sensor error: " + std::string(e.what()) << std::endl;
        current_value = 0.0;
    }
}