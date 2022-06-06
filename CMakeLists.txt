cmake_minimum_required(VERSION 3.22)
project(SolarProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(SolarProject WIN32 src/main.cpp headers/engine_structs.h src/engine_structs.cpp headers/engine_methods.h src/engine_methods.cpp headers/engine_globals.h src/SolarObject.cpp headers/SolarObject.h)

