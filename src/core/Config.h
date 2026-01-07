#ifndef CONFIG_H
#define CONFIG_H

#include "Logger.h"
#include <memory>
#include <string>
#include <yaml-cpp/yaml.h>

struct SensorConfig {
    std::string name;
    unsigned threshold;
    unsigned cores = 1;
};

struct ServerConfig {
    std::string name;
};

struct MonitoringConfig {
    unsigned interval_seconds;
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
    void loadConfigValues(const YAML::Node &config);
    void validateValues();
    void printConfigInfo(const std::string &filename);
};

#endif