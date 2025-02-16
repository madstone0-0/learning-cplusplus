cmake_minimum_required(VERSION 3.20)

project(Modern LANGUAGES CXX)

add_subdirectory(decltype)
add_executable(misc misc.cpp)
