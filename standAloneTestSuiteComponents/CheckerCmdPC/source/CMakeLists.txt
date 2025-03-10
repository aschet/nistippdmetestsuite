cmake_minimum_required(VERSION 3.15)

project(checkerCmd)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
    "checkerCmd.cpp"
    "checkerCmd.h"
    "IppCmdRes.cpp"
    "IppCmdRes.h"
    "parserCmd.cpp"
    "parserCmd.h"
    "tools.h"
    "world.cpp"
    "world.h"
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    PARSER_MAIN
)
