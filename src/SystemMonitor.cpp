#include "SystemMonitor.h"
#include <iostream>
#include <string>

SystemMonitor::SystemMonitor(const std::string& name, int cores)
    : serverName(name), cpuCores(cores), cycleCounter(0) {
    cpuLoad = 0.0;
    ramUsage = 0.0;
    diskUsage = 0.0;
    isHealthy = true;
}

void SystemMonitor::fetchData() {
    cycleCounter++;
    cpuLoad = 15.5 + (cycleCounter % 5) * 10;
    ramUsage = 42.3 + cycleCounter;
    diskUsage = 78.9;
    isHealthy = (cpuLoad < 90.0);
}

void SystemMonitor::displayStatus() {
  std::cout << "=== SYSTEM MONITOR ===" << std::endl;
  std::cout << "Server Name: " << serverName << std::endl;
  std::cout << "CPU Load: " << cpuLoad << "% (" << cpuCores << " cores)"
            << std::endl;
  std::cout << "RAM Usage: " << ramUsage << "%" << std::endl;
  std::cout << "Disk Usage: " << diskUsage << "%" << std::endl;
  std::cout << "Status: " << (isHealthy ? "OK" : "PROBLEM") << std::endl; 
  std::cout << "---" << std::endl;
}

bool SystemMonitor::isSystemHealthy() const {
    return isHealthy;
}
