#include "Logger.h"
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

void Logger::log(const std::string &message, const std::string &logLevel) {
    try {
        if (!std::filesystem::exists("logs")) {
            std::filesystem::create_directory("logs");
        }

        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        auto tm_ptr = std::localtime(&now_c);
        if (!tm_ptr) {
            throw std::runtime_error("Failed to convert time");
        }

        std::ofstream logFile("logs/system.log", std::ios::app);
        if (!logFile.is_open()) {
            throw std::ios_base::failure("Failed to open log file");
        }

        logFile << "["
                << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S")
                << "] " << "[" << logLevel << "] " << message << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[ " << logLevel << " ] " << message
                  << "(Log error: " << e.what() << std::endl;
    }
}

void Logger::logError(const std::string &message) { log(message, "ERROR"); }