cmake_minimum_required(VERSION 3.14)

project(Learning LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++20")
# set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -fno-ident")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add_compile_options(
# "-Wall" "-Wextra" "-std=c++20" "-fmodules-ts" "-fconcepts"
# "$<$<CONFIG:DEBUG>: -g3;-ggdb;-Os>"
# "$<$<CONFIG:RELEASE>: -O3;-Os;-fno-ident>"
#     )
include_directories("src/Crash_Course/_includes")
include_directories("src/Math/_includes")
set(INCLUDE ${CMAKE_SOURCE_DIR}/src/Crash_Course/_includes)
set(MATH_INC ${CMAKE_SOURCE_DIR}/src/Math/_includes)
set(TIMER ${INCLUDE}/timer.hpp ${INCLUDE}/timer.cpp)
set(TRACER ${INCLUDE}/tracer.hpp ${INCLUDE}/tracer.cpp)
set(BYTE_HEADERS ${INCLUDE}/bytes.hpp ${INCLUDE}/bytes.cpp)
set(STOPWATCH ${INCLUDE}/stopwatch.hpp)
set(LINALG ${CMAKE_SOURCE_DIR}/src/Math/MLinalg)
# file(GLOB_RECURSE HEADERS "src/_includes/*.hpp")

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

#################$
# Origin Library #
##################

# include_directories("third_party/asutton/origin")

#################
# Boost Library #
#################
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions(-D_WIN32_WINNT=0x0501)
find_package(
    Boost
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
