#include "CpuSensor.h"
#include <fstream>
#include <iostream>
#include <sstream>

CpuSensor::CpuSensor(int cpuCores, double alertThreshold)
    : Sensor("CPU", alertThreshold), cores(cpuCores), prevUser(0), prevNice(0),
      prevSystem(0), prevIdle(0), prevTotal(0) {}

void CpuSensor::fetchData() {
    try {
        std::ifstream statFile("/proc/stat");

        if (!statFile.is_open()) {
            throw std::ios_base::failure("Cannot open /proc/stat");
        }

        std::string line;
        if (!std::getline(statFile, line)) {
            throw std::ios_base::failure("Failed to read from /proc/stat");
        }

        std::istringstream iss(line);
        std::string cpu;
        unsigned long long user, nice, system, idle, iowait, irq, softirq,
            steal;
        if (!(iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
              softirq >> steal)) {
            throw std::ios_base::failure("Failed to parse /proc/stat data");
        }

        unsigned long long total =
            user + nice + system + idle + iowait + irq + softirq + steal;

        if (prevTotal > 0) {
            auto totalDiff = total - prevTotal;

            if (totalDiff < 0) {
                std::cerr << "CPU counter overflow detected, resetting"
                          << std::endl;
                prevTotal = total;
                return;
            }

            if (totalDiff == 0) {
                currentValue = prevTotal;
                return;
            } else {
                auto idleDiff = idle - prevIdle;
                currentValue = 100.0 * (1.0 - (double)idleDiff / totalDiff);
                currentValue = std::max(0.0, std::min(100.0, currentValue));
            }
        }

        prevUser = user;
        prevNice = nice;
        prevSystem = system;
        prevIdle = idle;
        prevTotal = total;

        std::cout << "CPU data collected from /proc/stat (" << (int)currentValue
                  << "%)" << std::endl;
    } catch (const std::ios_base::failure &e) {
        std::cerr << "Failed to read /proc/stat: " + std::string(e.what())
                  << std::endl;
        currentValue = 0.0;
    } catch (const std::exception &e) {
        std::cerr << "CPU Sensor error: " + std::string(e.what()) << std::endl;
        currentValue = 0.0;
    }
}