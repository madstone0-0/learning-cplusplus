cmake_minimum_required(VERSION 3.14)

project(smart_pointers LANGUAGES CXX)

add_executable(scoped scoped.cpp ${CATCH})
add_executable(unique unique.cpp ${CATCH})
add_executable(custom_deleter custom_deleter.cpp)
add_executable(shared shared.cpp ${CATCH})
add_executable(weak weak.cpp ${CATCH})
add_executable(intrusive intrusive.cpp ${CATCH})
add_executable(allocators allocators.cpp ${CATCH})
add_executable(hal hal.cpp)
