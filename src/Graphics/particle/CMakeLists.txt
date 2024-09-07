cmake_minimum_required(VERSION 3.14)

project(particle LANGUAGES CXX)
include_directories(${LINALG})
include_directories(${PARTICLE_INCLUDES})

set(INPUT
    ${GRAPHIC_INCUDES}/InputHandler.cpp
    ${GRAPHIC_INCUDES}/InputHandler.hpp
)
set(SCREEN ${PARTICLE_INCLUDES}/Screen.hpp ${PARTICLE_INCLUDES}/Screen.cpp)

set(BACKEND
    ${PARTICLE_INCLUDES}/Backend.hpp
    ${PARTICLE_INCLUDES}/SDL_Backend.hpp
    ${PARTICLE_INCLUDES}/SFML_Backend.hpp
)

set(SIM ${INPUT} ${BACKEND} ${SCREEN})

add_executable(sim sim.cpp ${SIM})

target_link_libraries(
    sim
    PRIVATE
        SDL2::SDL2
        PkgConfig::SDL2_IMAGE
        PkgConfig::SDL2_GFX
        mlinalg
        sfml-graphics
        ImGui-SFML::ImGui-SFML
        imgui
)
