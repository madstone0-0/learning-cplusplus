cmake_minimum_required(VERSION 3.14)

project(netowrking LANGUAGES CXX)

add_executable(asio asio.cpp)
add_executable(ip ip.cpp)
add_executable(abuffers abuffers.cpp)
add_executable(http_client http_client.cpp)
add_executable(r_w r_w.cpp)
add_executable(async_r_w async_r_w.cpp)
add_executable(server server.cpp)
add_executable(async_server async_server.cpp)
add_executable(multi_async_server multi_async_server.cpp)
add_executable(server_udp server_udp.cpp)
add_executable(daytime daytime.cpp)
add_executable(ping ping.cpp)
add_executable(beast beast.cpp)
add_executable(beast_http beast_http.cpp)
add_executable(async_beast_client async_beast_client.cpp)

target_link_libraries(abuffers PRIVATE Catch2::Catch2WithMain)

