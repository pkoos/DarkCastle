cmake_minimum_required(VERSION 3.14)
project(DarkCastle)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(googletest)

include_directories(googletest/include)

add_executable( main_test test/main_test.cpp )

target_link_libraries( main_test PRIVATE gtest_main )

include(GoogleTest)
gtest_discover_tests(main_test)