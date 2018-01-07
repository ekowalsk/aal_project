cmake_minimum_required(VERSION 3.9)
project(AAL)

set(CMAKE_CXX_STANDARD 11)

add_executable(AAL main.cpp Cube.cpp Cube.h Table.cpp Table.h Raster.cpp Raster.h Interface.cpp)