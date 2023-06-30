cmake_minimum_required(VERSION 3.14)

project(misc LANGUAGES CXX)
set(BYTE_HEADERS ${INCLUDE}/bytes.hpp ${INCLUDE}/bytes.cpp)

add_executable(algos algos.cpp ${INCLUDE}/algorithms.hpp ${TIMER})
# add_executable(algos algos.cpp)
# target_include_directories(algos INTERFACE ${INCLUDE})
add_executable(bytes bytes_main.cpp ${BYTE_HEADERS})
add_executable(bytes_test bytes_test.cpp ${BYTE_HEADERS} ${CATCH})
add_executable(diagSum diagSum.cpp)
add_executable(fast_inv_sqrt fast_inv_sqrt.cpp)
