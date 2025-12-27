#include <gtest/gtest.h>
#include "Sensor.h"
#include "CpuSensor.h"
#include "DiskSensor.h"
#include "RamSensor.h"
#include "SystemMonitor.h"

TEST(CpuSensorTest, FetchDataIncreasesValue) {
    CpuSensor cpu(8, 85.0);
    cpu.fetchData();
    EXPECT_GT(cpu.getValue(), 0.0);
}

TEST(RamSensorTest, AlertThreshold) {
    RamSensor ram(50.0);
    ram.fetchData();
    EXPECT_GT(ram.getValue(), 30.0);
    EXPECT_TRUE(ram.isHealthy());
}

TEST(DiskSensorTest, HighUsageTriggersAlarm) {
    DiskSensor disk(60.0);
    EXPECT_EQ(disk.getValue(), 0.0);

    disk.fetchData();
    EXPECT_TRUE(disk.isHealthy());

    disk.fetchData();
    EXPECT_GT(disk.getValue(), 50.0);
    EXPECT_FALSE(disk.isHealthy());
}

TEST(SensorTest, DisplayStatusOutput) {
    CpuSensor cpu(8, 85.0);
    cpu.fetchData();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

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