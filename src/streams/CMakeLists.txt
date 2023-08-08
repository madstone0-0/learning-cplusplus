cmake_minimum_required(VERSION 3.14)

project(streams LANGUAGES CXX)

add_executable(streams streams.cpp)
add_executable(state state.cpp)
add_executable(buffering buffering.cpp)
add_executable(user user.cpp)
add_executable(string string.cpp)
add_executable(file file.cpp)
add_executable(buffers buffers.cpp)
add_executable(autobrake autobrake.cpp)
add_executable(caps caps.cpp)
add_executable(fileinfo fileinfo.cpp)
# add_executable(fileinfo_boost fileinfo_boost.cpp)

target_link_libraries(file PRIVATE Catch2::Catch2WithMain)
target_link_libraries(streams PRIVATE Catch2::Catch2WithMain)
target_link_libraries(string PRIVATE Catch2::Catch2WithMain)
