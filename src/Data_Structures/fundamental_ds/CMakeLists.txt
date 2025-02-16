cmake_minimum_required(VERSION 3.20)

project(fundamental_ds LANGUAGES CXX)

include_directories(${DS})
add_executable(ds ds.cpp ${DS} ${TIMER} ${UTILS})
add_executable(ds_tests ds_tests.cpp ${DS} ${TIMER} ${CATCH})

target_link_libraries(ds_tests PRIVATE Catch2::Catch2WithMain)
