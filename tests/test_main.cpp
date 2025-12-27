#include <gtest/gtest.h>
#include "Sensor.h"
#include "CpuSensor.h"
#include "RamSensor.h"

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}