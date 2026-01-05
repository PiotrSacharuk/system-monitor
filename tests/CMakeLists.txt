add_executable(unit_tests test_main.cpp)
target_link_libraries(unit_tests
    PRIVATE core_lib sensors_lib core_headers sensors_headers yaml-cpp
    PRIVATE GTest::gtest GTest::gtest_main
)

add_test(NAME AllTests COMMAND unit_tests)