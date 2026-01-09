#ifndef CONFIG_H
#define CONFIG_H

#include "logger.h"

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

struct LoggerConfig {
    bool enabled;
    std::string level;
    std::string file_path;
};

class Config {
  public:
    static Config& get_instance();

    ServerConfig server;
    MonitoringConfig monitoring;
    SensorConfig cpu, ram, disk;
    LoggerConfig logging;

  private:
    Config();
    void load_from_file(const std::string& filename);
    void load_config_values(const YAML::Node& config);
    void validate_values();
    void print_config_info(const std::string& filename);
};

#endif