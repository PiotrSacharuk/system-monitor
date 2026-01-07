#include "cpu_sensor.h"

#include <fstream>
#include <iostream>
#include <sstream>

CpuSensor::CpuSensor(int cpu_cores, double alert_threshold)
    : Sensor("CPU", alert_threshold), cores(cpu_cores), prev_user(0), prev_nice(0), prev_system(0),
      prev_idle(0), prev_total(0) {}

void CpuSensor::fetch_data() {
    try {
        std::ifstream stat_file("/proc/stat");

        if (!stat_file.is_open()) {
            throw std::ios_base::failure("Cannot open /proc/stat");
        }

        std::string line;
        if (!std::getline(stat_file, line)) {
            throw std::ios_base::failure("Failed to read from /proc/stat");
        }

        std::istringstream iss(line);
        std::string cpu;
        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
        if (!(iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal)) {
            throw std::ios_base::failure("Failed to parse /proc/stat data");
        }

        unsigned long long total = user + nice + system + idle + iowait + irq + softirq + steal;

        if (prev_total > 0) {
            auto total_diff = total - prev_total;

            if (total_diff < 0) {
                std::cerr << "CPU counter overflow detected, resetting" << std::endl;
                prev_total = total;
                return;
            }

            if (total_diff == 0) {
                current_value = prev_total;
                return;
            } else {
                auto idleDiff = idle - prev_idle;
                current_value = 100.0 * (1.0 - (double)idleDiff / total_diff);
                current_value = std::max(0.0, std::min(100.0, current_value));
            }
        }

        prev_user = user;
        prev_nice = nice;
        prev_system = system;
        prev_idle = idle;
        prev_total = total;

        std::cout << "CPU data collected from /proc/stat (" << (int)current_value << "%)"
                  << std::endl;
    } catch (const std::ios_base::failure& e) {
        std::cerr << "Failed to read /proc/stat: " + std::string(e.what()) << std::endl;
        current_value = 0.0;
    } catch (const std::exception& e) {
        std::cerr << "CPU Sensor error: " + std::string(e.what()) << std::endl;
        current_value = 0.0;
    }
}