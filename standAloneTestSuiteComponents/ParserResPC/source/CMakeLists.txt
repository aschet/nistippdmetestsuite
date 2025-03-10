cmake_minimum_required(VERSION 3.15)

project(parserRes)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    "IppCmdRes.cpp"
    "IppCmdRes.h"
    "parserRes.cpp"
    "parserRes.h"
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    PARSER_MAIN
)
