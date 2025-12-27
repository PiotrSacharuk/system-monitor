#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include "Sensor.h"
#include <vector>
#include <memory>
#include <string>

class SystemMonitor {
private:
    std::string serverName;
    std::vector<std::unique_ptr<Sensor>> sensors;

public:
    SystemMonitor(const std::string& name);
    void addSensor(std::unique_ptr<Sensor> sensor);
    void fetchAllData();
    void displayStatus();
    bool isHealthy() const;
};

#endif