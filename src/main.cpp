#include "SystemMonitor.h"
#include "CpuSensor.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main() {
  std::string serverName = "Prod-Sever-01";
  int cpuCores = 8;

  SystemMonitor systemMonitor(serverName);
  systemMonitor.addSensor(std::make_unique<CpuSensor>(cpuCores, 85.0));

  for (int i = 0; i < 10; i++) {
    systemMonitor.fetchAllData();
    systemMonitor.displayStatus();
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  std::cout << "Monitoring finished." << std::endl;
  return 0;
}
