cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(Rail VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(EXEC main)
file(GLOB SOURCES src/*.cpp)

add_executable(${EXEC}
    main.cpp
    ${SOURCES}
)