cmake_minimum_required(VERSION 3.8)
project(FiniteAutomata)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/node.cpp src/node.h src/automata.cpp src/automata.h)
add_executable(FiniteAutomata ${SOURCE_FILES})