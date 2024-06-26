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
add_executable(donut donut.cpp)
add_executable(timer_test timer_test.cpp ${TIMER})
add_executable(utils_test utils_test.cpp)
add_executable(algos_test algos_test.cpp ${INCLUDE}/algorithms.hpp ${TIMER})
add_executable(dorg dorg.cpp)
add_executable(randHist randHist.cpp)


target_link_libraries(algos_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(bytes_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(timer_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(utils_test PRIVATE Catch2::Catch2WithMain)
