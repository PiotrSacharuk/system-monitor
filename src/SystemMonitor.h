#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <string>

class SystemMonitor {
private:
    std::string serverName;
    int cpuCores;
    double cpuLoad;
    double ramUsage;
    double diskUsage;
    bool isHealthy;
    int cycleCounter;

public:
    SystemMonitor(const std::string& name, int cores);

    void fetchData();
    void displayStatus();
    bool isSystemHealthy() const;
};

#endif