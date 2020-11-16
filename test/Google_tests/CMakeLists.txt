cmake_minimum_required(VERSION 3.16)

project(Google_tests)
add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run
        unit_tests/events/eventsSystemTest.cpp
)

target_include_directories(Google_Tests_run PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(Google_Tests_run gtest gtest_main)