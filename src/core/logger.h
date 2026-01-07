#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
  public:
    static void log(const std::string& message, const std::string& log_level = "INFO");
    static void log_error(const std::string& message);
};

#endif