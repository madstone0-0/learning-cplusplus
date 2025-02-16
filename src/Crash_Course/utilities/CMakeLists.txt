cmake_minimum_required(VERSION 3.20)

project(expressions LANGUAGES CXX)

add_executable(data_structs data_structs.cpp)
add_executable(date_time date_time.cpp ${TIMER})
add_executable(numerics numerics.cpp)
add_executable(randPass randPass.cpp)
add_executable(narrow_cast_test narrow_cast_test.cpp)

target_link_libraries(data_structs PRIVATE Catch2::Catch2WithMain)
target_link_libraries(date_time PRIVATE Catch2::Catch2WithMain)
target_link_libraries(narrow_cast_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(numerics PRIVATE Catch2::Catch2WithMain)
