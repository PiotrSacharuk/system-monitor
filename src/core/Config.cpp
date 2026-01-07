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
    cpu = {"CPU", 85, 8};
    ram = {"RAM", 90};
    disk = {"DISK", 90};

    loadFromFile("config.yaml");
    validateValues();
}

void Config::loadFromFile(const std::string &filename) {
    try {
        YAML::Node config = YAML::LoadFile(filename);
        loadConfigValues(config);
        printConfigInfo(const_cast<std::string &>(filename));
    } catch (const YAML::BadFile &e) {
        Logger::logError("Failed to load " + filename);

        try {
            const std::string exampleFilename = "config.example.yaml";
            Logger::log("Trying fallback: " + exampleFilename);
            YAML::Node config = YAML::LoadFile(exampleFilename);
            loadConfigValues(config);
            printConfigInfo(exampleFilename);
        } catch (const YAML::BadFile &e2) {
            Logger::logError(
                "Both config files missing, using built-in defaults");
        }
    } catch (const YAML::ParserException &e) {
        std::cerr << "YAML syntax error: " + std::string(e.what()) << std::endl;
    }
}

void Config::loadConfigValues(const YAML::Node &config) {
    if (config["server"]["name"]) {
        server.name = config["server"]["name"].as<std::string>();
    }

    if (config["monitoring"]["interval_seconds"]) {
        monitoring.interval_seconds =
            config["monitoring"]["interval_seconds"].as<unsigned>();
    }
    if (config["monitoring"]["test_cycles"]) {
        monitoring.test_cycles = config["monitoring"]["test_cycles"].as<int>();
    }

    if (config["sensors"]["cpu"]["threshold"]) {
        cpu.threshold = config["sensors"]["cpu"]["threshold"].as<unsigned>();
    }
    if (config["sensors"]["cpu"]["cores"]) {
        cpu.cores = config["sensors"]["cpu"]["cores"].as<unsigned>();
    }

    if (config["sensors"]["ram"]["threshold"]) {
        ram.threshold = config["sensors"]["ram"]["threshold"].as<unsigned>();
    }

    if (config["sensors"]["disk"]["threshold"]) {
        disk.threshold = config["sensors"]["disk"]["threshold"].as<unsigned>();
    }
}

void Config::validateValues() {
    if (cpu.cores <= 0) {
        throw std::invalid_argument("CPU cores must be > 0");
    }

    if (cpu.threshold < 0 || cpu.threshold > 100) {
        throw std::invalid_argument("CPU threshold must be 0-100");
    }

    if (ram.threshold < 0 || ram.threshold > 100) {
        throw std::invalid_argument("RAM threshold must be 0-100");
    }

    if (disk.threshold < 0 || disk.threshold > 100) {
        throw std::invalid_argument("DISK threshold must be 0-100");
    }

    if (monitoring.interval_seconds <= 0) {
        throw std::invalid_argument("Interval must be > 0");
    }
}

void Config::printConfigInfo(const std::string &filename) {
    Logger::log("Configuration from " + filename + ":");
    Logger::log("   Server: " + server.name);
    Logger::log("   Monitoring interval: " +
                std::to_string(monitoring.interval_seconds) + " seconds");
    Logger::log("   Monitoring test cycles: " +
                std::to_string(monitoring.test_cycles));
    Logger::log("   CPU: " + std::to_string(cpu.threshold) + "% (" +
                std::to_string(cpu.cores) + " cores)");
    Logger::log("   RAM: " + std::to_string(ram.threshold) + "%");
    Logger::log("   DISK: " + std::to_string(disk.threshold) + "%");
}