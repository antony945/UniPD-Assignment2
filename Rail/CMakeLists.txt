cmake_minimum_required(VERSION 3.5)
project(Rail LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB SOURCES src/*.cpp)

add_library(Railway ${SOURCES})
add_executable(RailwayTester main.cpp)
target_link_libraries(RailwayTester Railway)

# add_compile_options(-O2)
target_compile_options(RailwayTester PUBLIC -O2)