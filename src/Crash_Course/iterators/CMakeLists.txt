cmake_minimum_required(VERSION 3.20)

project(iterators LANGUAGES CXX)

add_executable(output output.cpp)
add_executable(input input.cpp)
add_executable(aux aux.cpp)
add_executable(prev prev.cpp)
add_executable(sumItr sumItr.cpp)
add_executable(perf perf.cpp ${TIMER})

target_link_libraries(aux PRIVATE Catch2::Catch2WithMain)
target_link_libraries(input PRIVATE Catch2::Catch2WithMain)
target_link_libraries(output PRIVATE Catch2::Catch2WithMain)
target_link_libraries(prev PRIVATE Catch2::Catch2WithMain)
target_link_libraries(sumItr PRIVATE Catch2::Catch2WithMain)
