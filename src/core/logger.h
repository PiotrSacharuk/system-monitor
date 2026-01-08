#ifndef LOGGER_H
#define LOGGER_H

#include <deque>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

struct LogEntry {
    std::string level;
    std::string message;
    std::string timestamp;

    friend std::ostream& operator<<(std::ostream& os, const LogEntry& entry) {
        os << "[" << entry.timestamp << "] [" << entry.level << "] " << entry.message;
        return os;
    }
};

class Logger {
  private:
    static constexpr size_t MAX_LOGS = 100;
    static inline std::string log_file_path = "system.log";
    static std::deque<LogEntry> log_buffer;
    static std::mutex log_mutex;

    static std::string get_timestamp();
    static void log(const std::string& message, const std::string& log_level = "INFO");
    static void write_to_file(const LogEntry& entry);

  public:
    static void init(const std::string& log_file_path);
    static void log_debug(const std::string& message);
    static void log_info(const std::string& message);
    static void log_error(const std::string& message);
    static void log_warning(const std::string& message);

    static std::vector<LogEntry> get_recent_logs(size_t count = 10);
    static std::vector<LogEntry> get_logs_by_level(const std::string& level);
    static size_t get_log_count();

    static void clear_logs();
    static void export_logs_to_file(const std::string& filename);
    static void print_all_logs();
};

#endif