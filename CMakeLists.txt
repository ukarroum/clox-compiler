cmake_minimum_required(VERSION 3.27)
project(clox_compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(clox_compiler
        main.cpp
        debug.cpp
        chunk.cpp
        VM.cpp
        Scanner.cpp
        compiler.cpp
        value.cpp)
add_executable(test_compiler
        tests/lox_tester.cpp)