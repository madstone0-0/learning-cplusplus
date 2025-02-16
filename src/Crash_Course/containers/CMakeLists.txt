cmake_minimum_required(VERSION 3.20)

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

target_link_libraries(associative PRIVATE Catch2::Catch2WithMain)
target_link_libraries(graphs PRIVATE Catch2::Catch2WithMain)
target_link_libraries(init_list PRIVATE Catch2::Catch2WithMain)
target_link_libraries(matrix PRIVATE Catch2::Catch2WithMain)
target_link_libraries(sequence PRIVATE Catch2::Catch2WithMain)
