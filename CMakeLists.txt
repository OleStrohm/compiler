cmake_minimum_required(VERSION 3.8)
project(FiniteAutomata)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/automata/node.cpp src/automata/node.h src/automata/automata.cpp src/automata/automata.h)
add_executable(FiniteAutomata ${SOURCE_FILES})