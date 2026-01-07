#include "sensor.h"

#include <iostream>

Sensor::Sensor(const std::string& sensor_name, double alert_threshold)
    : name(sensor_name), threshold(alert_threshold), current_value(0.0) {}

void Sensor::display_status() const {
    std::cout << "[" << name << "] " << current_value << "% " << (is_healthy() ? "OK" : "PROBLEM")
              << std::endl;
}