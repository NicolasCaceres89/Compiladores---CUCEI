cmake_minimum_required(VERSION 3.10)

project(LexerAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lexer-analyzer
    src/main.cpp
    src/lexer/afd.cpp
    src/synthesizer/parser.cpp
    src/synthesizer/instruction.cpp
    src/control_unit/moore_fsm.cpp
)

target_include_directories(lexer-analyzer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/include/synthesizer
    ${CMAKE_CURRENT_SOURCE_DIR}/include/control_unit
)
