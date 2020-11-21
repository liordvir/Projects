cmake_minimum_required(VERSION 3.15)
project(matamFinal)

set(CMAKE_CXX_STANDARD 14)

add_executable(matamFinal main.cpp Auxiliaries.cpp Auxiliaries.h Graph.cpp Graph.h Gcalc.cpp Gcalc.h Exceptions.cpp Exceptions.h Arithmetic.cpp Arithmetic.h PythonInterface.cpp PythonInterface.h)