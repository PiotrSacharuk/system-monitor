cmake_minimum_required(VERSION 3.10)

project(SystemMonitor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
include_directories(src)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

add_executable(SystemMonitor
    src/main.cpp
    src/SystemMonitor.cpp
    src/Sensor.cpp
    src/CpuSensor.cpp
    src/RamSensor.cpp
    src/DiskSensor.cpp
)

add_executable(unit_tests
    tests/test_main.cpp
    src/SystemMonitor.cpp
    src/Sensor.cpp
    src/CpuSensor.cpp
    src/RamSensor.cpp
    src/DiskSensor.cpp
)
target_link_libraries(unit_tests gtest gtest_main)
add_test(NAME AllTests COMMAND unit_tests)
