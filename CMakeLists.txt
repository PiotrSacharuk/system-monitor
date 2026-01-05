cmake_minimum_required(VERSION 3.25)
project(SystemMonitor VERSION 1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

find_package(yaml-cpp 0.7 QUIET)
if(NOT yaml-cpp_FOUND)
    message(STATUS "yaml-cpp not found, using FetchContent fallback...")
    include(FetchContent)
    FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG        yaml-cpp-0.7.0
    )
    FetchContent_MakeAvailable(yaml-cpp)
endif()

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    message(STATUS "GTest not found, using FetchContent fallback...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR DEB)
include(CPack)