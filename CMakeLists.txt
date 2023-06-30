cmake_minimum_required(VERSION 3.14)

project(Learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++20")
include_directories("src/_includes")
set(INCLUDE ${CMAKE_SOURCE_DIR}/src/_includes)
set(TIMER ${INCLUDE}/timer.hpp ${INCLUDE}/timer.cpp)
set(TRACER ${INCLUDE}/tracer.hpp ${INCLUDE}/tracer.cpp)
set(CATCH ${INCLUDE}/catch_amalgamated.hpp ${INCLUDE}/catch_amalgamated.cpp)
# file(GLOB_RECURSE HEADERS "src/_includes/*.hpp")

#################
# IceCream      #
#################

find_package(IcecreamCpp)
include_directories(${IcecreamCpp_INCLUDE_DIRS})

#################$
# Origin Library #
##################

# include_directories("third_party/asutton/origin")

#################
# Boost Library #
#################
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
add_definitions(-D_WIN32_WINNT=0x0501)
find_package(Boost COMPONENTS regex date_time system filesystem unit_test_framework program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

macro(boosttest_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} PRIVATE ${Boost_FILESYSTEM_LIBRARY}
                                PRIVATE ${Boost_SYSTEM_LIBRARY}
                                PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

#######################
# Google Test Library #
#######################

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

# add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/google/googletest" "third_party/google/googletest")
# mark_as_advanced(
#     BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
#     gmock_build_tests gtest_build_samples gtest_build_tests
#     gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
# )
#
# set_target_properties(gtest PROPERTIES FOLDER third_party)
# set_target_properties(gtest_main PROPERTIES FOLDER third_party)
# set_target_properties(gmock PROPERTIES FOLDER third_party)
# set_target_properties(gmock_main PROPERTIES FOLDER third_party)
#
# macro(googletest_add_test TESTNAME)
#     add_executable(${TESTNAME} ${ARGN})
#     target_link_libraries(${TESTNAME} PRIVATE gtest PRIVATE gmock PRIVATE gtest_main)
#     add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
#     set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
# endmacro()



add_subdirectory(src)
