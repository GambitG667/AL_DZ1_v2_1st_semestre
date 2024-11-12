cmake_minimum_required(VERSION 3.10)

project(Calculator_v.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

add_executable(calculator
    src/main.cpp
    src/Token.hpp
    src/Token.cpp
    src/Calculator.hpp
    src/Calculator.cpp
)