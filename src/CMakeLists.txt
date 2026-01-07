add_library(core_headers INTERFACE)
target_include_directories(core_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/core)

add_library(sensors_headers INTERFACE)
target_include_directories(sensors_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sensors)

add_library(core_lib STATIC
    core/Config.cpp
    core/SystemMonitor.cpp
    core/Logger.cpp
)
target_link_libraries(core_lib
    PUBLIC
        yaml-cpp::yaml-cpp
        sensors_headers
)
target_include_directories(core_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core)

add_library(sensors_lib STATIC
    sensors/Sensor.cpp
    sensors/CpuSensor.cpp
    sensors/RamSensor.cpp
    sensors/DiskSensor.cpp
)
target_link_libraries(sensors_lib
    PUBLIC
        core_lib
        sensors_headers
)

add_executable(SystemMonitor main.cpp)
target_link_libraries(SystemMonitor PRIVATE core_lib sensors_lib yaml-cpp)

# Coverage
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(SystemMonitor PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_options(SystemMonitor PRIVATE -fprofile-arcs -ftest-coverage)
endif()

install(TARGETS SystemMonitor RUNTIME DESTINATION bin)
