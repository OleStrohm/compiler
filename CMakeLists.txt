cmake_minimum_required(VERSION 3.8)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

file(GLOB RES_FILES res/*)
file(COPY ${RES_FILES} DESTINATION "${PROJECT_SOURCE_DIR}/cmake-build-debug")

set(SOURCE_FILES src/main.cpp src/tokenizer/node.cpp src/tokenizer/node.h src/tokenizer/automata.cpp src/tokenizer/automata.h src/tokenizer/Regex.cpp src/tokenizer/Regex.h src/tokenizer/regex.cpp src/tokenizer/regex.h src/tokenizer/tokenizer.cpp src/tokenizer/tokenizer.h src/tokenizer/token.h)
add_executable(Compiler ${SOURCE_FILES})
