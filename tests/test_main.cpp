#include "CpuSensor.h"
#include "DiskSensor.h"
#include "RamSensor.h"
#include "Sensor.h"
#include "SystemMonitor.h"
#include <gtest/gtest.h>

TEST(CpuSensorTest, RealMetricsPositive) {
    CpuSensor cpu(8, 85.0);
    double before = cpu.getValue();
    cpu.fetchData();
    double after = cpu.getValue();
    EXPECT_GE(after, 0.0);
}

TEST(RamSensorTest, RealMetricsReasonable) {
    RamSensor ram(50.0);
    ram.fetchData();
    EXPECT_GT(ram.getValue(), 0.0);
    EXPECT_LE(ram.getValue(), 99.0);
    EXPECT_TRUE(ram.isHealthy());
}

TEST(DiskSensorTest, ThresholdWorks) {
    DiskSensor disk(50.0);
    disk.fetchData();

    double value = disk.getValue();
    EXPECT_EQ(disk.isHealthy(), value < 50.0);
}

TEST(SensorTest, DisplayStatusOutput) {
    CpuSensor cpu(8, 85.0);
    cpu.fetchData();

    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    cpu.displayStatus();

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_TRUE(output.find("[CPU]") != std::string::npos);
    EXPECT_TRUE(output.find("OK") != std::string::npos);
}

TEST(SystemMonitorTest, PolymorphismWorks) {
    SystemMonitor monitor("TestServer");
    monitor.addSensor(std::make_unique<CpuSensor>(4, 90.0));
    monitor.addSensor(std::make_unique<RamSensor>(80.0));

    EXPECT_TRUE(monitor.isHealthy());

    monitor.fetchAllData();
    monitor.displayStatus();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}