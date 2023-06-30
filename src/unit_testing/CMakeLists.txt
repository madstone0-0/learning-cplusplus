cmake_minimum_required(VERSION 3.14)

project(expressions LANGUAGES CXX)

add_executable(car_test_catch car_test_catch.cpp ${CATCH})
add_executable(car_test car_test.cpp)
add_executable(car_test_google car_test_google.cpp)

enable_testing()

target_link_libraries(
  car_test_google
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(car_test_google)
