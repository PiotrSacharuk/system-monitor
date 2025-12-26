#include <iostream>
#include <string>

int main() {
  std::string serverName = "Prod-Server-01";
  int cpuCores = 8;
  double cpuLoad = 15.5;
  double ramUsage = 42.3;
  double diskUsage = 78.9;
  bool isHealthy = true;

  std::cout << "=== SYSTEM MONITOR v1.0 ===" << std::endl;
  std::cout << "Server Name: " << serverName << std::endl;
  std::cout << "CPU Load: " << cpuLoad << "% (" << cpuCores << " cores)"
            << std::endl;
  std::cout << "RAM Usage: " << ramUsage << "%" << std::endl;
  std::cout << "Disk Usage: " << diskUsage << "%" << std::endl;
  std::cout << "Status: " << (isHealthy ? "OK" : "PROBLEM") << std::endl;

  return 0;
}