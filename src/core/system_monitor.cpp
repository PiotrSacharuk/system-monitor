#include "system_monitor.h"

#include "logger.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>

SystemMonitor::SystemMonitor(const std::string& name) : server_name(name) {}

void SystemMonitor::add_sensor(std::unique_ptr<Sensor> sensor) {
    std::string name = sensor->get_name();
    sensors[name] = std::move(sensor);
    history[name] = {};
}

void SystemMonitor::fetch_all_data() {
    std::cout << "Collect data from all sensors..." << std::endl;
    for (auto& [name, sensor] : sensors) {
        try {
            sensor->fetch_data();

            auto& buf = history[name];

            buf.push_back(sensor->get_value());
            if (buf.size() > HISTORY_SIZE) {
                buf.pop_front();
            }
        } catch (const std::exception& e) {
            Logger::log_error("Sensor " + name + " failed: " + std::string(e.what()));
            auto& buf = history[name];
            if (!buf.empty()) {
                buf.push_back(buf.back());
            } else {
                buf.push_back(0.0);
            }
        }
    }
}

double SystemMonitor::get_sensor_average(const std::string& name) const {
    auto it = history.find(name);
    if (it == history.end() || it->second.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (double value : it->second) {
        sum += value;
    }

    return sum / it->second.size();
}

double SystemMonitor::get_sensor_max(const std::string& name) const {
    auto it = history.find(name);
    if (it == history.end() || it->second.empty())
        return 0.0;

    const auto& buf = it->second;
    return *std::max_element(buf.begin(), buf.end());
}

double SystemMonitor::get_sensor_trend(const std::string& name) const {
    auto it = history.find(name);
    if (it == history.end())
        return 0.0;

    const auto& buf = it->second;
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

void SystemMonitor::display_status() {
    std::cout << "=== SYSTEM MONITOR ===" << std::endl;
    std::cout << "Server: " << server_name << std::endl;

    bool all_healthy = true;
    for (const auto& sensor : sensors) {
        sensor.second->display_status();
        if (!sensor.second->is_healthy()) {
            all_healthy = false;
            std::string msg = sensor.second->get_name() +
                              " usage critical: " + std::to_string(sensor.second->get_value());
            Logger::log_error(msg);
        }
    }

    std::cout << "Overall status: " << (all_healthy ? "OK" : "ALARM") << std::endl;
    std::cout << "---" << std::endl;
}

bool SystemMonitor::is_healthy() const {
    for (const auto& sensor : sensors) {
        if (!sensor.second->is_healthy())
            return false;
    }
    return true;
}
