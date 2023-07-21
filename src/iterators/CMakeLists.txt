cmake_minimum_required(VERSION 3.14)

project(iterators LANGUAGES CXX)

add_executable(output output.cpp)
add_executable(input input.cpp)
add_executable(aux aux.cpp)
add_executable(prev prev.cpp)
add_executable(sumItr sumItr.cpp)
add_executable(perf perf.cpp ${TIMER})
