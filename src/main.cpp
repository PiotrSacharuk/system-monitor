#include "core/Config.h"
#include "core/Logger.h"
#include "core/SystemMonitor.h"
#include "sensors/CpuSensor.h"
#include "sensors/DiskSensor.h"
#include "sensors/RamSensor.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main() {
    try {
        auto &config = Config::getInstance();
        Logger::log("System Monitor started: " + config.server.name);

        SystemMonitor systemMonitor(config.server.name);
        systemMonitor.addSensor(
            std::make_unique<CpuSensor>(config.cpu.cores, config.cpu.threshold));
        systemMonitor.addSensor(std::make_unique<RamSensor>(config.ram.threshold));
        systemMonitor.addSensor(
            std::make_unique<DiskSensor>(config.disk.threshold));

        Logger::log("Monitoring " + std::to_string(config.monitoring.test_cycles) +
                    " cycles every " +
                    std::to_string(config.monitoring.interval_seconds) + "s");

        int cycles = config.monitoring.test_cycles;
        if (cycles == -1) {
            Logger::log("PRODUCTION MODE: Infinite monitoring");
            while (true) {
                systemMonitor.fetchAllData();
                systemMonitor.displayStatus();

                std::cout << "\n STATISTICS: " << std::endl;
                std::cout << "CPU avg: " << systemMonitor.getSensorAverage("CPU")
                        << std::endl;
                std::cout << "CPU max: " << systemMonitor.getSensorMax("CPU")
                        << std::endl;
                std::cout << "CPU trend: " << systemMonitor.getSensorTrend("CPU")
                        << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        } else {
            Logger::log("TEST MODE: " + std::to_string(cycles) + " cycles.");
            for (int i = 0; i < cycles; i++) {
                systemMonitor.fetchAllData();
                systemMonitor.displayStatus();
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }

        std::cout << "Monitoring finished." << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
