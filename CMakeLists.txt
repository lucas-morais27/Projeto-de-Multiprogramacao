set( CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe" )
set( CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe" )
cmake_minimum_required(VERSION 3.5)
project(auxiliar)

include_directories( include )
set (CMAKE_CXX_FLAGS "-Wall")

add_executable( auxiliar
                src/Auxiliar.cpp
                )