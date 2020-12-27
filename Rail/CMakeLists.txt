cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(Rail VERSION 1.0 LANGUAGES CXX)

include_directories(include)

add_library(Station 
    include/Station.h
    src/Station.cpp
)

add_library(Train 
    include/Train.h
    src/Train.cpp
)

add_executable(main
    src/main.cpp
)

target_link_libraries(main
    Station
    Train
)
