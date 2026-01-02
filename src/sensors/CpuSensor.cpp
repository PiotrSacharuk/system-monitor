#include "CpuSensor.h"
#include <fstream>
#include <iostream>
#include <sstream>

CpuSensor::CpuSensor(int cpuCores, double alertThreshold)
    : Sensor("CPU", alertThreshold), cores(cpuCores), prevUser(0), prevNice(0),
      prevSystem(0), prevIdle(0), prevTotal(0) {}

void CpuSensor::fetchData() {
    std::ifstream statFile("/proc/stat");
    std::string line;
    std::getline(statFile, line);

    std::istringstream iss(line);
    std::string cpu;
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
        steal;

    unsigned long long total =
        user + nice + system + idle + iowait + irq + softirq + steal;

    if (prevTotal > 0) {
        auto totalDiff = total - prevTotal;
        auto idleDiff = idle - prevIdle;
        currentValue = 100.0 * (1.0 - (double)idleDiff / totalDiff);
    }

    prevUser = user;
    prevNice = nice;
    prevSystem = system;
    prevIdle = idle;
    prevTotal = total;

    std::cout << "CPU data collected from /proc/stat (" << (int)currentValue
              << "%)" << std::endl;
}