cmake_minimum_required(VERSION 3.8)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

file(GLOB RES_FILES res/*)
file(COPY ${RES_FILES} DESTINATION "${PROJECT_SOURCE_DIR}/cmake-build-debug")

set(SOURCE_FILES src/main.cpp src/tokenizer/node.cpp src/tokenizer/node.h src/tokenizer/automaton.cpp src/tokenizer/automaton.h src/tokenizer/regex.cpp src/tokenizer/regex.h src/tokenizer/regex.cpp src/tokenizer/regex.h src/tokenizer/tokenizer.cpp src/tokenizer/tokenizer.h src/tokenizer/token.h src/syntax/contextFreeGrammar.cpp src/syntax/contextFreeGrammar.h src/util/util.cpp src/util/util.h src/syntax/ast.cpp src/syntax/ast.h src/syntax/CFGTree.cpp src/syntax/CFGTree.h src/syntax/expansion.cpp src/syntax/expansion.h)
add_executable(Compiler ${SOURCE_FILES})
