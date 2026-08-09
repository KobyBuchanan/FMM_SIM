cmake_minimum_required(VERSION 3.21)
project(FMM_SIM CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.cpp
    src/quadtree.cpp
)

find_package(SFML CONFIG REQUIRED COMPONENTS System Window Graphics)
add_executable(FMM_SIM ${SOURCES})
target_include_directories(FMM_SIM PRIVATE src)

target_link_libraries(FMM_SIM PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
)