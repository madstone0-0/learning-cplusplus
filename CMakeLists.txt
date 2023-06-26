cmake_minimum_required(VERSION 3.14)

project(Learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++20")
include_directories("src/_includes")
include_directories("src")

add_subdirectory(src)
