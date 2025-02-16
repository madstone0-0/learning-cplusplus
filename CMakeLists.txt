cmake_minimum_required(VERSION 3.20)

project(Learning LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
add_compile_options(-Wall -Wextra -Werror=return-type)
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -fsanitize=address")

set(CMAKE_CXX_FLAGS_RELEASE "-Os -fno-ident")
set(CMAKE_CXX_FLAGS_PROFILE "-g -pg")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("src/Crash_Course/_includes")
include_directories("src/Math/_includes")
include_directories("src/Data_Structures/_includes")
set(INCLUDE ${CMAKE_SOURCE_DIR}/src/Crash_Course/_includes)
set(FUND_INC ${CMAKE_SOURCE_DIR}/src/Data_Structures/_includes)
# set(DS ${FUND_INC}/fundamental.hpp ${FUND_INC}/fundamental_interfaces.hpp)
file(
    GLOB DS
    # CONFIGURE_DEPENDS
    ${FUND_INC}/interfaces/*.hpp
    ${FUND_INC}/structures/*.hpp
)
set(MATH_INC ${CMAKE_SOURCE_DIR}/src/Math/_includes)
set(TIMER ${INCLUDE}/timer.hpp ${INCLUDE}/timer.cpp)
set(TRACER ${INCLUDE}/tracer.hpp ${INCLUDE}/tracer.cpp)
set(BYTE_HEADERS ${INCLUDE}/bytes.hpp ${INCLUDE}/bytes.cpp)
set(STOPWATCH ${INCLUDE}/stopwatch.hpp)

include(FetchContent)

#################
# PkgConfig     #
#################
find_package(PkgConfig REQUIRED)
pkg_search_module(TBB REQUIRED tbb IMPORTED_TARGET)

#################
#  Fmt          #
#################
find_package(fmt 10 REQUIRED)

#################
# Python        #
#################
# pkg_search_module(PYTHON REQUIRED python3 IMPORTED_TARGET)
find_package(Python3 COMPONENTS Interpreter Development NumPy)
include_directories(${PYTHON_INCLUDE_DIRS})

#################
# Catch2        #
#################
find_package(Catch2 3 REQUIRED)

#################
# IceCream      #
#################

find_package(IcecreamCpp)
include_directories(${IcecreamCpp_INCLUDE_DIRS})

#######################
# Matplot++           #
#######################
find_package(Matplot++ REQUIRED)

#######################
# SDL2                #
#######################
find_package(SDL2 REQUIRED)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image IMPORTED_TARGET)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer IMPORTED_TARGET)
pkg_search_module(SDL2_GFX REQUIRED SDL2_gfx IMPORTED_TARGET)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

#################
# SFML          #
#################
# set(SFML_DIR "/usr/lib/cmake/SFML") # Move this above find_package
# set(BUILD_SHARED_LIBS OFF) # Disable shared libs
# set(SFML_STATIC_LIBRARIES TRUE)
# find_package(SFML 2.6 COMPONENTS graphics system window REQUIRED CONFIG)
# pkg_search_module(SFML REQUIRED sfml-all IMPORTED_TARGET)
set(SFML_VERSION 2.6.2)
FetchContent_Declare(
    SFML
    URL "https://github.com/SFML/SFML/archive/${SFML_VERSION}.zip"
)
option(SFML_BUILD_AUDIO "Build audio" OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(sfml)

######################
# ImGui & ImGui-Sfml #
######################
set(IMGUI_VERSION 1.91.1)
# set(IMGUI_SFML_VERSION 2.3)

FetchContent_Declare(
    imgui
    URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
)

FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG 2.6.x
)

# Dear ImGui
FetchContent_MakeAvailable(imgui)
# set(IMGUI_DIR "/usr/include")
# find_package(imgui REQUIRED)

# ImGui-SFML
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" OFF)

# find_package(ImGui-SFML REQUIRED CONFIG)
FetchContent_MakeAvailable(imgui-sfml)

#################
# MLinalg       #
#################
find_package(mlinalg REQUIRED)

#################
# Boost Library #
#################
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions(-D_WIN32_WINNT=0x0501)
find_package(
    Boost
    CONFIG
    COMPONENTS
        regex
        date_time
        system
        filesystem
        unit_test_framework
        program_options
    REQUIRED
)
include_directories(${Boost_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES})

macro(boosttest_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(
        ${TESTNAME}
        PRIVATE ${Boost_FILESYSTEM_LIBRARY}
        PRIVATE ${Boost_SYSTEM_LIBRARY}
        PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

#######################
# Google Test Library #
#######################

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(
    "${PROJECT_SOURCE_DIR}/third_party/google/googletest"
    "third_party/google/googletest"
)
mark_as_advanced(
    BUILD_GMOCK
    BUILD_GTEST
    BUILD_SHARED_LIBS
    gmock_build_tests
    gtest_build_samples
    gtest_build_tests
    gtest_disable_pthreads
    gtest_force_shared_crt
    gtest_hide_internal_symbols
)

set_target_properties(gtest PROPERTIES FOLDER third_party)
set_target_properties(gtest_main PROPERTIES FOLDER third_party)
set_target_properties(gmock PROPERTIES FOLDER third_party)
set_target_properties(gmock_main PROPERTIES FOLDER third_party)

macro(googletest_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(
        ${TESTNAME}
        PRIVATE gtest
        PRIVATE gmock
        PRIVATE gtest_main
    )
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

add_subdirectory(src)
