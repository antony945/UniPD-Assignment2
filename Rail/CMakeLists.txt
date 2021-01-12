cmake_minimum_required(VERSION 3.5)
project(Rail LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB SOURCES src/*.cpp)

add_library(Railway ${SOURCES})
add_executable(RailwayTester main.cpp)
target_link_libraries(RailwayTester Railway)

if(MSVC)
    target_compile_options(RailwayTester PUBLIC /O2)
else()
    target_compile_options(RailwayTester PUBLIC -O2)
endif()
# set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O2")