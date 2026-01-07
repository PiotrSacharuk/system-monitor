#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include "sensor.h"

#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class SystemMonitor {
  private:
    std::string server_name;
    std::unordered_map<std::string, std::unique_ptr<Sensor>> sensors;
    std::unordered_map<std::string, std::deque<double>> history;
    static constexpr size_t HISTORY_SIZE = 10;

  public:
    SystemMonitor(const std::string& name);
    void add_sensor(std::unique_ptr<Sensor> sensor);
    double get_sensor_average(const std::string& name) const;
    double get_sensor_max(const std::string& name) const;
    double get_sensor_trend(const std::string& name) const;
    std::vector<double> get_sensor_history(const std::string& name) const;
    void fetch_all_data();
    void display_status();
    bool is_healthy() const;
};

#endif