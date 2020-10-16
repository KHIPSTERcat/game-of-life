cmake_minimum_required(VERSION 3.16)
project(untitled1)

set(CMAKE_CXX_STANDARD 14)

add_library(sfml INTERFACE)

add_definitions(-DSFML_DYNAMIC)

target_include_directories(sfml INTERFACE "G:/SFML-2.5.1/include")
target_link_libraries(sfml INTERFACE "G:/SFML-2.5.1/lib/sfml-graphics.lib" "G:/SFML-2.5.1/lib/sfml-window.lib" "G:/SFML-2.5.1/lib/sfml-system.lib")

add_executable(untitled1 main.cpp)

target_link_libraries(untitled1 sfml)