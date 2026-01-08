#include "logger.h"

#include <fstream>
#include <gtest/gtest.h>
#include <thread>

class LoggerTest : public ::testing::Test {
  protected:
    void SetUp() override { Logger::clear_logs(); }
};

TEST_F(LoggerTest, LogInfo) {
    Logger::log_info("This is an info message");
    auto logs = Logger::get_recent_logs(1);
    ASSERT_EQ(logs.size(), 1);
    EXPECT_EQ(logs[0].level, "INFO");
    EXPECT_EQ(logs[0].message, "This is an info message");
}

TEST_F(LoggerTest, RingBufferLimit) {
    for (size_t i = 0; i < 150; ++i) {
        Logger::log_info("Log message " + std::to_string(i));
    }
    auto log_count = Logger::get_log_count();
    EXPECT_EQ(log_count, 100); // MAX_LOGS is 100

    auto logs = Logger::get_recent_logs(100);
    EXPECT_EQ(logs[0].message, "Log message 50");   // Oldest log should be "Log message 50"
    EXPECT_EQ(logs[99].message, "Log message 149"); // Newest log should be "Log message 149"
}

TEST_F(LoggerTest, ErrorLogging) {
    Logger::log_error("Error occurred");
    Logger::log_warning("Warning");
    Logger::log_info("Info");

    auto errors = Logger::get_logs_by_level("ERROR");
    auto warns = Logger::get_logs_by_level("WARNING");

    EXPECT_EQ(errors.size(), 1);
    EXPECT_EQ(warns.size(), 1);
}

TEST_F(LoggerTest, ThreadSafety) {
    std::vector<std::thread> threads;

    for (int t = 0; t < 10; ++t) {
        threads.emplace_back([t]() {
            for (int i = 0; i < 10; ++i) {
                Logger::log_info("Thread " + std::to_string(t) + " msg " + std::to_string(i));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(Logger::get_log_count(), 100);
}

TEST_F(LoggerTest, ExportToFile) {
    Logger::log_info("Export test 1");
    Logger::log_error("Export test 2");

    const std::string filename = "/tmp/test_logs.txt";
    Logger::export_logs_to_file(filename);

    std::ifstream file(filename);
    EXPECT_TRUE(file.good());
    file.close();
}