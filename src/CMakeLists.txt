add_library(core_headers INTERFACE)
target_include_directories(core_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/core)

add_library(sensors_headers INTERFACE)
target_include_directories(sensors_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sensors)

add_library(core_lib STATIC
    core/config.cpp
    core/system_monitor.cpp
    core/logger.cpp
)
target_link_libraries(core_lib
    PUBLIC
        yaml-cpp::yaml-cpp
        sensors_headers
)
target_include_directories(core_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core)

add_library(sensors_lib STATIC
    sensors/sensor.cpp
    sensors/cpu_sensor.cpp
    sensors/ram_sensor.cpp
    sensors/disk_sensor.cpp
)
target_link_libraries(sensors_lib
    PUBLIC
        core_lib
        sensors_headers
)

add_executable(system-monitor main.cpp)
target_link_libraries(system-monitor PRIVATE core_lib sensors_lib yaml-cpp)

# Coverage
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(system-monitor PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_options(system-monitor PRIVATE -fprofile-arcs -ftest-coverage)
endif()

install(TARGETS system-monitor RUNTIME DESTINATION bin)