#include "SystemMonitor.h"
#include "Logger.h"
#include <iostream>
#include <string>

SystemMonitor::SystemMonitor(const std::string& name) : serverName(name) {}

void SystemMonitor::addSensor(std::unique_ptr<Sensor> sensor) {
    sensors.push_back(std::move(sensor));
}

void SystemMonitor::fetchAllData() {
    std::cout << "Collect data from all sensors..." << std::endl;
    for (auto& sensor : sensors) {
        sensor->fetchData();
    }
}

void SystemMonitor::displayStatus() {
    std::cout << "=== SYSTEM MONITOR ===" << std::endl;
    std::cout << "Server: " << serverName << std::endl;

    bool allHealthy = true;
    for (const auto& sensor : sensors) {
        sensor->displayStatus();
        if (!sensor->isHealthy()) {
            allHealthy = false;
            std::string msg = sensor->getName() + " usage critical: " + std::to_string(sensor->getValue());
            Logger::logError(msg);
        }
    }

    std::cout << "Overall status: " << (allHealthy ? "OK" : "ALARM") << std::endl;
    std::cout << "---" << std::endl;
}

bool SystemMonitor::isHealthy() const {
    for (const auto& sensor : sensors) {
        if (!sensor->isHealthy())
            return false;
    }
    return true;
}
