#include "Logger.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

void Logger::log(const std::string& message, const std::string& logLevel) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    
    std::ofstream logFile("logs/system.log", std::ios::app);
    
    if (logFile.is_open()) {
        logFile << "[" << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S")
            << "] " << "[" << logLevel << "] " << message << std::endl;
    }
}

void Logger::logError(const std::string& message) {
    log(message, "ERROR");
}