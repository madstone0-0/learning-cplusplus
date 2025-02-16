cmake_minimum_required(VERSION 3.20)

project(expressions LANGUAGES CXX)

add_executable(compound compound.cpp ${TRACER})
add_executable(declaration declaration.cpp)
add_executable(Fibonacci Fibonacci.cpp ${INCLUDE}/fib.hpp ${INCLUDE}/fib.cpp)
add_executable(iteration iteration.cpp)
add_executable(jump jump.cpp)
add_executable(namespaces namespaces.cpp)
add_executable(Prime Prime.cpp ${INCLUDE}/prime.hpp ${INCLUDE}/prime.cpp)
add_executable(selection selection.cpp)
