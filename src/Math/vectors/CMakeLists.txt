cmake_minimum_required(VERSION 3.14)

project(vectors LANGUAGES CXX)

add_executable(2d 2d.cpp ${MATH_UTIL})
add_executable(3d 3d.cpp ${MATH_UTIL})

target_link_libraries(
    2d
    PRIVATE Python3::Python Python3::NumPy fmt::fmt Matplot++::matplot
)
target_link_libraries(
    3d
    PRIVATE Python3::Python Python3::NumPy fmt::fmt Matplot++::matplot
)
