cmake_minimum_required(VERSION 3.14)

project(concurrecny LANGUAGES CXX)

add_executable(async async.cpp)
add_executable(factorize factorize.cpp ${TIMER})
add_executable(race race.cpp)
add_executable(mutex mutex.cpp)
add_executable(atomics atomics.cpp)
add_executable(condition condition.cpp)
add_executable(parallel parallel.cpp ${TIMER})
add_executable(algos_par algos_par.cpp)
add_executable(spin_lock spin_lock.cpp)
add_executable(thread_queue thread_queue.cpp)

target_link_libraries(async PRIVATE Catch2::Catch2WithMain)
target_link_libraries(algos_par PRIVATE PkgConfig::TBB PkgConfig::TBB)
target_link_libraries(parallel PRIVATE PkgConfig::TBB PkgConfig::TBB)
