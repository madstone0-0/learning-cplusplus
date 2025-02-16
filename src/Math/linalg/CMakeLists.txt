cmake_minimum_required(VERSION 3.20)

project(linalg LANGUAGES CXX)

add_executable(tests tests.cpp)
add_executable(fundamental fundamental.cpp)
target_link_libraries(tests mlinalg)
target_link_libraries(fundamental mlinalg)
