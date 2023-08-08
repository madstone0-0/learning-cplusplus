cmake_minimum_required(VERSION 3.14)

project(strings LANGUAGES CXX)

add_executable(std_string std_string.cpp)
add_executable(string_view string_view.cpp)
add_executable(regex regex.cpp)
add_executable(boost boost.cpp)
add_executable(histogram histogram.cpp)
add_executable(palindrome palindrome.cpp)
add_executable(vowels vowels.cpp)


target_link_libraries(boost PRIVATE Catch2::Catch2WithMain)
target_link_libraries(regex PRIVATE Catch2::Catch2WithMain)
target_link_libraries(std_string PRIVATE Catch2::Catch2WithMain)
target_link_libraries(string_view PRIVATE Catch2::Catch2WithMain)
