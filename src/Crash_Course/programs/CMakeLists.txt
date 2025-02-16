cmake_minimum_required(VERSION 3.20)

project(programs LANGUAGES CXX)

add_executable(support support.cpp)
add_executable(program_options program_options.cpp)
add_executable(preprocessor preprocessor.cpp)
add_executable(async_server_graceful async_server_graceful.cpp)
add_executable(async_client_prg async_client_prg.cpp)
add_executable(mgrep mgrep.cpp)

# target_compile_features(mgrep PRIVATE cxx_std_23)
