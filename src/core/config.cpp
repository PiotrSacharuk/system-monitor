#include "config.h"

#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

Config& Config::get_instance() {
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
    logging = {true, "INFO", "system.log"};

    load_from_file("config.yaml");
    validate_values();
}

void Config::load_from_file(const std::string& filename) {
    try {
        YAML::Node config = YAML::LoadFile(filename);
        load_config_values(config);
        print_config_info(const_cast<std::string&>(filename));
        Logger::init(logging.file_path);
    } catch (const YAML::BadFile& e) {
        Logger::log_error("Failed to load " + filename);

        try {
            const std::string example_filename = "config.example.yaml";
            Logger::log_info("Trying fallback: " + example_filename);
            YAML::Node config = YAML::LoadFile(example_filename);
            load_config_values(config);
            print_config_info(example_filename);
            Logger::init(logging.file_path);
        } catch (const YAML::BadFile& e2) {
            Logger::log_error("Both config files missing, using built-in defaults");
        }
    } catch (const YAML::ParserException& e) {
        std::cerr << "YAML syntax error: " + std::string(e.what()) << std::endl;
    }
}

void Config::load_config_values(const YAML::Node& config) {
    if (config["server"]["name"]) {
        server.name = config["server"]["name"].as<std::string>();
    }

    if (config["monitoring"]["interval_seconds"]) {
        monitoring.interval_seconds = config["monitoring"]["interval_seconds"].as<unsigned>();
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

    if (config["logging"]["enabled"]) {
        logging.enabled = config["logging"]["enabled"].as<bool>();
        if (!logging.enabled) {
            logging.file_path = "";
        } else {
            if (config["logging"]["file_path"]) {
                logging.file_path = config["logging"]["file_path"].as<std::string>();
                std::cout << "Log file_path set to " + logging.file_path << std::endl;
            }
            if (config["logging"]["level"]) {
                logging.level = config["logging"]["level"].as<std::string>();
                std::cout << "Log level set to " + logging.level << std::endl;
            }
        }
    }
}

void Config::validate_values() {
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

void Config::print_config_info(const std::string& filename) {
    Logger::log_info("Configuration from " + filename + ":");
    Logger::log_info("   Server: " + server.name);
    Logger::log_info("   Monitoring interval: " + std::to_string(monitoring.interval_seconds) +
                     " seconds");
    Logger::log_info("   Monitoring test cycles: " + std::to_string(monitoring.test_cycles));
    Logger::log_info("   CPU: " + std::to_string(cpu.threshold) + "% (" +
                     std::to_string(cpu.cores) + " cores)");
    Logger::log_info("   RAM: " + std::to_string(ram.threshold) + "%");
    Logger::log_info("   DISK: " + std::to_string(disk.threshold) + "%");
}