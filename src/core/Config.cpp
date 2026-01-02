#include "Config.h"
#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

Config &Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config() {
    // Default values
    server.name = "Default-Server";
    monitoring = {5, 12};
    cpu = {"CPU", 85.0, 8};
    ram = {"RAM", 90.0};
    disk = {"DISK", 90.0};

    loadFromFile("config.yaml");
}

void Config::loadFromFile(const std::string &filename) {
    try {
        YAML::Node config = YAML::LoadFile(filename);

        if (config["server"]["name"]) {
            server.name = config["server"]["name"].as<std::string>();
        }

        if (config["monitoring"]["interval_seconds"]) {
            monitoring.interval_seconds =
                config["monitoring"]["interval_seconds"].as<int>();
        }
        if (config["monitoring"]["test_cycles"]) {
            monitoring.test_cycles =
                config["monitoring"]["test_cycles"].as<int>();
        }

        if (config["sensors"]["cpu"]["threshold"]) {
            cpu.threshold = config["sensors"]["cpu"]["threshold"].as<double>();
        }
        if (config["sensors"]["cpu"]["cores"]) {
            cpu.cores = config["sensors"]["cpu"]["cores"].as<int>();
        }

        if (config["sensors"]["ram"]["threshold"]) {
            ram.threshold = config["sensors"]["ram"]["threshold"].as<double>();
        }

        if (config["sensors"]["disk"]["threshold"]) {
            disk.threshold =
                config["sensors"]["disk"]["threshold"].as<double>();
        }

        std::cout << "Config loaded from " << filename << std::endl;
        std::cout << "   Server: " << server.name << std::endl;
        std::cout << "   CPU: " << cpu.threshold << "% (" << cpu.cores
                  << " cores)" << std::endl;
    } catch (const YAML::BadFile &e) {
        std::cout << "config.yaml not found, using defaults: " << e.what()
                  << std::endl;
    } catch (const YAML::ParserException &e) {
        std::cout << "YAML parse error: " << e.what() << std::endl;
    }
}