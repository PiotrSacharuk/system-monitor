#include "logger.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

std::deque<LogEntry> Logger::log_buffer;
std::mutex Logger::log_mutex;

std::string Logger::get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0')
       << std::setw(3) << ms.count();
    return ss.str();
}

void Logger::log(const std::string& message, const std::string& log_level) {
    std::lock_guard<std::mutex> lock(log_mutex);

    LogEntry entry{log_level, message, get_timestamp()};
    log_buffer.push_back(entry);
    if (log_buffer.size() > MAX_LOGS) {
        log_buffer.pop_front();
    }

    std::cout << entry << std::endl;
    write_to_file(entry);
}

void Logger::write_to_file(const LogEntry& entry) {
    try {
        std::filesystem::path log_path(log_file_path);
        auto parent = log_path.parent_path();
        if (!parent.empty()) {
            std::filesystem::create_directories(parent);
        }

        std::ofstream file(log_file_path, std::ios::app);
        if (!file.is_open()) {
            return;
        }
        file << entry << std::endl;
        file.close();
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "[ERROR] Filesystem error: " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "[ERROR] " << ex.what() << std::endl;
    }
}

void Logger::init(const std::string& log_file_path) {
    Logger::log_file_path = log_file_path;
    std::cout << "[INFO] Logger initialized with log file: " << log_file_path << std::endl;
}

void Logger::log_debug(const std::string& message) {
    log(message, "DEBUG");
}

void Logger::log_info(const std::string& message) {
    log(message, "INFO");
}

void Logger::log_error(const std::string& message) {
    log(message, "ERROR");
}

void Logger::log_warning(const std::string& message) {
    log(message, "WARNING");
}

std::vector<LogEntry> Logger::get_recent_logs(size_t count) {
    std::lock_guard<std::mutex> lock(log_mutex);
    std::vector<LogEntry> recent_logs;
    size_t start = log_buffer.size() > count ? log_buffer.size() - count : 0;

    for (size_t i = start; i < log_buffer.size(); ++i) {
        recent_logs.push_back(log_buffer[i]);
    }
    return recent_logs;
}

std::vector<LogEntry> Logger::get_logs_by_level(const std::string& level) {
    std::lock_guard<std::mutex> lock(log_mutex);
    std::vector<LogEntry> filtered_logs;

    for (const auto& entry : log_buffer) {
        if (entry.level == level) {
            filtered_logs.push_back(entry);
        }
    }
    return filtered_logs;
}

size_t Logger::get_log_count() {
    std::lock_guard<std::mutex> lock(log_mutex);
    return log_buffer.size();
}

void Logger::clear_logs() {
    std::lock_guard<std::mutex> lock(log_mutex);
    log_buffer.clear();
    std::cout << "[INFO] Log buffer cleared" << std::endl;
}

void Logger::export_logs_to_file(const std::string& filename) {
    std::lock_guard<std::mutex> lock(log_mutex);

    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        log("Failed to open log file: " + filename, "ERROR");
        return;
    }

    for (const auto& entry : log_buffer) {
        file << entry << std::endl;
    }
    file.close();
    std::cout << "[INFO] Logs exported to " << filename << " (" << log_buffer.size() << " entries)"
              << std::endl;
}

void Logger::print_all_logs() {
    std::lock_guard<std::mutex> lock(log_mutex);

    std::cout << "\n========== LOG BUFFER (" << log_buffer.size()
              << " entries) ==========" << std::endl;
    for (size_t i = 0; i < log_buffer.size(); ++i) {
        std::cout << "[" << i << "] " << log_buffer[i] << std::endl;
    }
    std::cout << "========================================\n" << std::endl;
}