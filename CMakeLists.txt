cmake_minimum_required(VERSION 3.10)

project(SystemMonitor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
include_directories(src src/core src/sensors)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

add_executable(SystemMonitor
    src/main.cpp
    src/core/SystemMonitor.cpp
    src/core/Logger.cpp
    src/sensors/Sensor.cpp
    src/sensors/CpuSensor.cpp
    src/sensors/RamSensor.cpp
    src/sensors/DiskSensor.cpp
)

add_executable(unit_tests
    tests/test_main.cpp
    src/core/SystemMonitor.cpp
    src/core/Logger.cpp
    src/sensors/Sensor.cpp
    src/sensors/CpuSensor.cpp
    src/sensors/RamSensor.cpp
    src/sensors/DiskSensor.cpp
)
target_link_libraries(unit_tests gtest gtest_main)
add_test(NAME AllTests COMMAND unit_tests)

# only for debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
set(CMAKE_C_EXE_LINKER_FLAGS_DEBUG "${CMAKE_C_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")

# Coverage raport
add_custom_target(coverage
    COMMAND mkdir -p coverage
    COMMAND lcov --directory . --capture --output-file coverage/coverage.info --ignore-errors mismatch
    COMMAND lcov --remove coverage/coverage.info '/usr/*' '*/googletest/*' --output-file coverage/coverage_filtered.info
    COMMAND genhtml coverage/coverage_filtered.info --output-directory coverage/html
    COMMAND echo "✅ Coverage report: file://$(pwd)/coverage/html/index.html"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating coverage report (ignoring mismatches)"
)
