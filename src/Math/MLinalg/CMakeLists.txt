cmake_minimum_required(VERSION 3.14)

project(MLinalg LANGUAGES CXX VERSION 1.0)

file(GLOB CPP CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB HPP CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/*.hpp)
include_directories(${PROJECT_SOURCE_DIR})

add_library(${PROJECT_NAME} SHARED ${CPP} ${HPP})
set_target_properties(MLinalg PROPERTIES LINKER_LANGUAGE CXX)
