cmake_minimum_required(VERSION 3.8)
project(FiniteAutomata)

set(CMAKE_CXX_STANDARD 11)

file(GLOB RES_FILES res/*)
file(COPY ${RES_FILES} DESTINATION "${PROJECT_SOURCE_DIR}/cmake-build-debug")

set(SOURCE_FILES src/main.cpp src/automata/node.cpp src/automata/node.h src/automata/automata.cpp src/automata/automata.h src/automata/Regex.cpp src/automata/Regex.h src/automata/regex.cpp src/automata/regex.h)
add_executable(FiniteAutomata ${SOURCE_FILES})
