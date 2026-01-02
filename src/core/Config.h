#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <string>

struct SensorConfig {
    std::string name;
    double threshold;
    int cores = 1;
};

struct ServerConfig {
    std::string name;
};

struct MonitoringConfig {
    int interval_seconds;
    int test_cycles;
};

class Config {
  public:
    static Config &getInstance();

    ServerConfig server;
    MonitoringConfig monitoring;
    SensorConfig cpu, ram, disk;

  private:
    Config();
    void loadFromFile(const std::string &filename);
};

#endif