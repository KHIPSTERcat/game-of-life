cmake_minimum_required(VERSION 3.16)
project(game-of-life)

set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(game-of-life-client client.cpp world.pb.cc world.pb.h)
target_link_libraries(game-of-life-client ${CONAN_LIBS})

add_executable(game-of-life-server server.cpp world.pb.cc world.pb.h)
target_link_libraries(game-of-life-server ${CONAN_LIBS})

