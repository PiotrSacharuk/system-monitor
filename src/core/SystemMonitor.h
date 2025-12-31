#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include "Sensor.h"
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class SystemMonitor
{
private:
    std::string serverName;
    std::unordered_map<std::string, std::unique_ptr<Sensor>> sensors;
    std::unordered_map<std::string, std::deque<double>> history;
    static constexpr size_t HISTORY_SIZE = 10;

public:
    SystemMonitor(const std::string &name);
    void addSensor(std::unique_ptr<Sensor> sensor);
    double getSensorAverage(const std::string &name) const;
    double getSensorMax(const std::string &name) const;
    double getSensorTrend(const std::string &name) const;
    std::vector<double> getSensorHistory(const std::string &name) const;
    void fetchAllData();
    void displayStatus();
    bool isHealthy() const;
};

#endif