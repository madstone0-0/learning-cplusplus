cmake_minimum_required(VERSION 3.14)

project(manage LANGUAGES CXX)

add_executable(basicMgt basicMgt.cpp ${TIMER})

target_link_libraries(basicMgt PRIVATE fmt::fmt)
