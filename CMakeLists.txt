cmake_minimum_required(VERSION 3.10)

project(SystemMonitor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(SystemMonitor src/main.cpp)

enable_testing()
add_test(NAME FetchTest COMMAND SystemMonitor --test-fetch)
