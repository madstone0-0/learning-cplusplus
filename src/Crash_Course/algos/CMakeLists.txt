cmake_minimum_required(VERSION 3.20)

project(algos LANGUAGES CXX)

add_executable(stdlib stdlib.cpp)
add_executable(sample sample.cpp)
add_executable(boost_algos boost_algos.cpp)
add_executable(ranges ranges.cpp)
add_executable(search_file search_file.cpp)

target_link_libraries(stdlib PRIVATE Catch2::Catch2WithMain)
target_link_libraries(boost_algos PRIVATE Catch2::Catch2WithMain)
target_link_libraries(ranges PRIVATE Catch2::Catch2WithMain)
