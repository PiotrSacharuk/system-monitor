#include <iostream>
#include <string>

void fetchSystemData(std::string& serverName, double& cpuLoad, double& ramUsage, double& diskUsage, bool& isHealthy) {
  serverName = "Prod-Server-01";
  cpuLoad = 15.5;
  ramUsage = 42.3;
  diskUsage = 78.9;
  isHealthy = true;
}

void displayStatus(const std::string& serverName, double cpuLoad, int cpuCores, double ramUsage, double diskUsage, bool isHealthy) {
  std::cout << "=== SYSTEM MONITOR v1.0 ===" << std::endl;
  std::cout << "Server Name: " << serverName << std::endl;
  std::cout << "CPU Load: " << cpuLoad << "% (" << cpuCores << " cores)"
            << std::endl;
  std::cout << "RAM Usage: " << ramUsage << "%" << std::endl;
  std::cout << "Disk Usage: " << diskUsage << "%" << std::endl;
  std::cout << "Status: " << (isHealthy ? "OK" : "PROBLEM") << std::endl; 
}

int main() {
  std::string serverName;
  double cpuLoad, ramUsage, diskUsage;
  bool isHealthy;
  int cpuCores = 8;

  fetchSystemData(serverName, cpuLoad, ramUsage, diskUsage, isHealthy);
  displayStatus(serverName, cpuLoad, cpuCores, ramUsage, diskUsage, isHealthy);

  return 0;
}
