#include "cpu_sensor.h"
#include "disk_sensor.h"
#include "ram_sensor.h"
#include "sensor.h"
#include "system_monitor.h"

#include <gtest/gtest.h>

TEST(CpuSensorTest, RealMetricsPositive) {
    CpuSensor cpu(8, 85.0);
    double before = cpu.get_value();
    cpu.fetch_data();
    double after = cpu.get_value();
    EXPECT_GE(after, 0.0);
}

TEST(RamSensorTest, RealMetricsReasonable) {
    RamSensor ram(50.0);
    ram.fetch_data();
    EXPECT_GT(ram.get_value(), 0.0);
    EXPECT_LE(ram.get_value(), 99.0);
    EXPECT_TRUE(ram.is_healthy());
}

TEST(DiskSensorTest, ThresholdWorks) {
    DiskSensor disk(50.0);
    disk.fetch_data();

    double value = disk.get_value();
    EXPECT_EQ(disk.is_healthy(), value < 50.0);
}

TEST(SensorTest, DisplayStatusOutput) {
    CpuSensor cpu(8, 85.0);
    cpu.fetch_data();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    cpu.display_status();

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_TRUE(output.find("[CPU]") != std::string::npos);
    EXPECT_TRUE(output.find("OK") != std::string::npos);
}

TEST(SystemMonitorTest, PolymorphismWorks) {
    SystemMonitor monitor("TestServer");
    monitor.add_sensor(std::make_unique<CpuSensor>(4, 90.0));
    monitor.add_sensor(std::make_unique<RamSensor>(80.0));

    EXPECT_TRUE(monitor.is_healthy());

    monitor.fetch_all_data();
    monitor.display_status();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}