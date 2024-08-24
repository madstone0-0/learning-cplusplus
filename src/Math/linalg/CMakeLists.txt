cmake_minimum_required(VERSION 3.14)

project(linalg LANGUAGES CXX)

include_directories(${LINALG})

add_executable(tests tests.cpp)
target_link_libraries(tests MLinalg)
