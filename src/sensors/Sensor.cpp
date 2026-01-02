#include "Sensor.h"
#include <iostream>

Sensor::Sensor(const std::string &sensorName, double alertThreshold)
    : name(sensorName), threshold(alertThreshold), currentValue(0.0) {}

void Sensor::displayStatus() const {
    std::cout << "[" << name << "] " << currentValue << "% "
              << (isHealthy() ? "OK" : "PROBLEM") << std::endl;
}