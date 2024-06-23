cmake_minimum_required(VERSION 3.14)

project(Math LANGUAGES CXX)

set(MATH_UTIL ${MATH_INC}/math_utils.hpp)

add_subdirectory(MLinalg)
add_subdirectory(linalg)
add_subdirectory(vectors)
