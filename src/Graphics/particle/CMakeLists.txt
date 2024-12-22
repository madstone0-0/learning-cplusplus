cmake_minimum_required(VERSION 3.14)

project(particle LANGUAGES CXX)
include_directories(${PARTICLE_INCLUDES})

set(INPUT
    ${GRAPHIC_INCUDES}/InputHandler.cpp
    ${GRAPHIC_INCUDES}/InputHandler.hpp
)
set(SCREEN ${PARTICLE_INCLUDES}/Screen.hpp ${PARTICLE_INCLUDES}/Screen.cpp)
set(PHYSICS ${PARTICLE_INCLUDES}/Physics.hpp)

set(SIM ${INPUT} ${SCREEN} ${PHYSICS})

add_executable(sim sim.cpp ${SIM})

target_link_libraries(
    sim
    PRIVATE
        ImGui-SFML::ImGui-SFML
        imgui
        SDL2::SDL2
        PkgConfig::SDL2_IMAGE
        PkgConfig::SDL2_GFX
        mlinalg::mlinalg
        sfml-graphics
)
