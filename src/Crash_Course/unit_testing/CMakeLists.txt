cmake_minimum_required(VERSION 3.20)

project(expressions LANGUAGES CXX)

set(TEST_INCLUDES ${INCLUDE}/mock_og.cpp)

add_executable(car_test_catch car_test_catch.cpp ${CATCH})
add_executable(car_test car_test.cpp)
googletest_add_test(car_test_google car_test_google.cpp)
add_executable(car_test_boost car_test_boost.cpp)

googletest_add_test(car_test_google_mock car_test_google_mock.cpp)
googletest_add_test(signs_test signs_test.cpp)

enable_testing()

target_link_libraries(car_test_catch PRIVATE Catch2::Catch2WithMain)

include(GoogleTest)
gtest_discover_tests(car_test_google)
gtest_discover_tests(car_test_google_mock)
gtest_discover_tests(signs_test)
