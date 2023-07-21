cmake_minimum_required(VERSION 3.14)

project(expressions LANGUAGES CXX)

add_executable(sequence sequence.cpp)
add_executable(associative associative.cpp)
add_executable(graphs graphs.cpp)
add_executable(init_list init_list.cpp)
add_executable(fibVec fibVec.cpp ${INCLUDE}/fib.hpp ${INCLUDE}/fib.cpp)
add_executable(rewrite rewrite.cpp)
add_executable(alphanumeric alphanumeric.cpp)
add_executable(fibCache fibCache.cpp)
add_executable(matrix matrix.cpp)
