cmake_minimum_required(VERSION 3.14)

project(Graphics LANGUAGES CXX)
set(GRAPHIC_INCUDES ${CMAKE_SOURCE_DIR}/src/Graphics/_includes)
set(PARTICLE_INCLUDES ${CMAKE_SOURCE_DIR}/src/Graphics/particle/include)
include_directories(${GRAPHIC_INCUDES})

add_subdirectory(particle)
