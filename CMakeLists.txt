cmake_minimum_required(VERSION 3.21)
project(FMM_SIM CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(SFML CONFIG REQUIRED COMPONENTS System Window Graphics)
add_executable(FMM_SIM src/main.cpp)

target_link_libraries(FMM_SIM PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
)