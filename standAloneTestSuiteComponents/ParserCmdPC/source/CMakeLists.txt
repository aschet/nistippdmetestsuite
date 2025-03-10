cmake_minimum_required(VERSION 3.15)

project(parserCmd)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    "IppCmdRes.cpp"
    "IppCmdRes.h"
    "parserCmd.cpp"
    "parserCmd.h"
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    PARSER_MAIN
)
