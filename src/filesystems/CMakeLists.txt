cmake_minimum_required(VERSION 3.14)

project(filesystems LANGUAGES CXX)

add_executable(filesystems filesystems.cpp)
add_executable(dirs dirs.cpp)
add_executable(iterators iterators.cpp)
add_executable(listdir listdir.cpp ${BYTE_HEADERS})
add_executable(treedir treedir.cpp ${BYTE_HEADERS})
add_executable(magic magic.cpp)
add_executable(search search.cpp)
add_executable(ls ls.cpp)
