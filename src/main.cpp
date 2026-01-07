#include "core/config.h"
#include "core/logger.h"
#include "core/system_monitor.h"
#include "sensors/cpu_sensor.h"
#include "sensors/disk_sensor.h"
#include "sensors/ram_sensor.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main() {
    try {
        auto& config = Config::get_instance();
        Logger::log("System Monitor started: " + config.server.name);

        SystemMonitor system_monitor(config.server.name);
        system_monitor.add_sensor(
            std::make_unique<CpuSensor>(config.cpu.cores, config.cpu.threshold));
        system_monitor.add_sensor(std::make_unique<RamSensor>(config.ram.threshold));
        system_monitor.add_sensor(std::make_unique<DiskSensor>(config.disk.threshold));

        Logger::log("Monitoring " + std::to_string(config.monitoring.test_cycles) +
                    " cycles every " + std::to_string(config.monitoring.interval_seconds) + "s");

        int cycles = config.monitoring.test_cycles;
        int interval = config.monitoring.interval_seconds;
        if (cycles == -1) {
            Logger::log("PRODUCTION MODE: Infinite monitoring");
            while (true) {
                try {
                    system_monitor.fetch_all_data();
                    system_monitor.display_status();

                    std::cout << "\n STATISTICS: " << std::endl;
                    std::cout << "CPU avg: " << system_monitor.get_sensor_average("CPU")
                              << std::endl;
                    std::cout << "CPU max: " << system_monitor.get_sensor_max("CPU") << std::endl;
                    std::cout << "CPU trend: " << system_monitor.get_sensor_trend("CPU")
                              << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(interval));
                } catch (const std::exception& e) {
                    std::cerr << "Monitoring error: " << std::string(e.what()) << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(interval));
                }
            }
        } else {
            Logger::log("TEST MODE: " + std::to_string(cycles) + " cycles.");
            for (int i = 0; i < cycles; i++) {
                system_monitor.fetch_all_data();
                system_monitor.display_status();
                std::this_thread::sleep_for(std::chrono::seconds(interval));
            }
        }

        std::cout << "Monitoring finished." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred." << std::endl;
        return 1;
    }
}
