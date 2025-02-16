cmake_minimum_required(VERSION 3.20)

project(threads LANGUAGES CXX)

add_executable(thread thread.cpp)

add_subdirectory(manage)
