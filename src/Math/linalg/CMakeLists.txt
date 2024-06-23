cmake_minimum_required(VERSION 3.14)

project(linalg LANGUAGES CXX)

include_directories(${LINALG})

add_executable(solver solver.cpp)
target_link_libraries(solver MLinalg)
