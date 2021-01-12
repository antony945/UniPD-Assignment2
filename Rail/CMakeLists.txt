cmake_minimum_required(VERSION 3.5)
project(Rail LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(EXEC main)
file(GLOB SOURCES src/*.cpp)

add_executable(${EXEC}
    main.cpp
    ${SOURCES}
)

# add_compile_options(-O2)
target_compile_options(${EXEC} PUBLIC -O2)