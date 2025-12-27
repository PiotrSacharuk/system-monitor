#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void fetchSystemData(std::string& serverName, double& cpuLoad, double& ramUsage, double& diskUsage, bool& isHealthy) {
  static int counter = 0;
  counter++;
  
  serverName = "Prod-Server-01";
  cpuLoad = 15.5 + (counter % 5) * 10;
  ramUsage = 42.3 + counter;
  diskUsage = 78.9;
  isHealthy = (cpuLoad < 90.0);
}

void displayStatus(const std::string& serverName, double cpuLoad, int cpuCores, double ramUsage, double diskUsage, bool isHealthy) {
  std::cout << "=== SYSTEM MONITOR ===" << std::endl;
  std::cout << "Server Name: " << serverName << std::endl;
  std::cout << "CPU Load: " << cpuLoad << "% (" << cpuCores << " cores)"
            << std::endl;
  std::cout << "RAM Usage: " << ramUsage << "%" << std::endl;
  std::cout << "Disk Usage: " << diskUsage << "%" << std::endl;
  std::cout << "Status: " << (isHealthy ? "OK" : "PROBLEM") << std::endl; 
  std::cout << "---" << std::endl;
}

int main() {
  std::string serverName;
  double cpuLoad, ramUsage, diskUsage;
  bool isHealthy;
  int cpuCores = 8;

  for (int i = 0; i < 10; i++) {
    fetchSystemData(serverName, cpuLoad, ramUsage, diskUsage, isHealthy);
    displayStatus(serverName, cpuLoad, cpuCores, ramUsage, diskUsage, isHealthy);
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  std::cout << "Monitoring finished." << std::endl;
  return 0;
}
