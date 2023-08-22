cmake_minimum_required(VERSION 3.24)
project(lox_compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(lox_compiler
        src/main.cpp src/TokenType.h src/Scanner.cpp src/Scanner.h src/Token.cpp src/Token.h src/Object.h)
