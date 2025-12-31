#include "SystemMonitor.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>

SystemMonitor::SystemMonitor(const std::string &name) : serverName(name) {}

void SystemMonitor::addSensor(std::unique_ptr<Sensor> sensor)
{
    std::string name = sensor->getName();
    sensors[name] = std::move(sensor);
    history[name] = {};
}

void SystemMonitor::fetchAllData()
{
    std::cout << "Collect data from all sensors..." << std::endl;
    for (auto &[name, sensor] : sensors)
    {
        sensor->fetchData();

        auto &buf = history[name];

        buf.push_back(sensor->getValue());
        if (buf.size() > HISTORY_SIZE)
        {
            buf.pop_front();
        }
    }
}

double SystemMonitor::getSensorAverage(const std::string &name) const
{
    auto it = history.find(name);
    if (it == history.end() || it->second.empty())
    {
        return 0.0;
    }

    double sum = 0.0;
    for (double value : it->second)
    {
        sum += value;
    }

    return sum / it->second.size();
}

double SystemMonitor::getSensorMax(const std::string &name) const
{
    auto it = history.find(name);
    if (it == history.end() || it->second.empty())
        return 0.0;

    const auto &buf = it->second;
    return *std::max_element(buf.begin(), buf.end());
}

double SystemMonitor::getSensorTrend(const std::string &name) const
{
    auto it = history.find(name);
    if (it == history.end())
        return 0.0;

    const auto &buf = it->second;
    const std::size_t n = buf.size();
    if (n < 4)
        return 0.0;

    const std::size_t window = std::min<std::size_t>(3, n / 2);

    auto old_begin = buf.begin();
    auto old_end = old_begin + window;
    auto recent_end = buf.end();
    auto recent_begin = recent_end - window;

    double old_sum = std::accumulate(old_begin, old_end, 0.0);
    double recent_sum = std::accumulate(recent_begin, recent_end, 0.0);

    double old_avg = old_sum / window;
    double recent_avg = recent_sum / window;

    if (old_avg == 0.0)
        return 0.0;
    return (recent_avg - old_avg) / old_avg * 100.0;
}

void SystemMonitor::displayStatus()
{
    std::cout << "=== SYSTEM MONITOR ===" << std::endl;
    std::cout << "Server: " << serverName << std::endl;

    bool allHealthy = true;
    for (const auto &sensor : sensors)
    {
        sensor.second->displayStatus();
        if (!sensor.second->isHealthy())
        {
            allHealthy = false;
            std::string msg = sensor.second->getName() + " usage critical: " + std::to_string(sensor.second->getValue());
            Logger::logError(msg);
        }
    }

    std::cout << "Overall status: " << (allHealthy ? "OK" : "ALARM") << std::endl;
    std::cout << "---" << std::endl;
}

bool SystemMonitor::isHealthy() const
{
    for (const auto &sensor : sensors)
    {
        if (!sensor.second->isHealthy())
            return false;
    }
    return true;
}
