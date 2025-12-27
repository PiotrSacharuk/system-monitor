#include "Config.h"
#include "SystemMonitor.h"
#include "CpuSensor.h"
#include "DiskSensor.h"
#include "RamSensor.h"
#include "Logger.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main() {
  Logger::log("System Monitor started.");
  std::string serverName = "Prod-Sever-01";
  int cpuCores = 8;

  SystemMonitor systemMonitor(serverName);
  systemMonitor.addSensor(std::make_unique<CpuSensor>(cpuCores, 85.0));
  systemMonitor.addSensor(std::make_unique<RamSensor>(90.0));
  systemMonitor.addSensor(std::make_unique<DiskSensor>(90.0));

  int cycles = Config::monitoringCycles();
  if (cycles == -1) {
    Logger::log("PRODUCTION MODE: Infinite monitoring");
    while (true) {
      systemMonitor.fetchAllData();
      systemMonitor.displayStatus();
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
}
