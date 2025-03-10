cmake_minimum_required(VERSION 3.15)

project(Server)

set(CMAKE_MFC_FLAG 2)

add_executable(${PROJECT_NAME} WIN32)

target_sources(${PROJECT_NAME}
  PRIVATE
    "res/bitmap1.bmp"
    "res/cursor1.cur"
    "res/Server.ico"
    "res/Server.rc2"
    "src/CmdContextChecker/checkerCmd.cpp"
    "src/CmdContextChecker/checkerCmd.h"
    "src/CmdParser/parserCmd.cpp"
    "src/CmdParser/parserCmd.h"
    "src/CmdResClasses/IppCmdRes.cpp"
    "src/CmdResClasses/IppCmdRes.h"
    "src/CMM/tools.h"
    "src/CMM/world.cpp"
    "src/CMM/world.h"
    "src/Executor/executor.cpp"
    "src/Executor/executor.h"
    "src/Server/dbg_mem.h"
    "src/Server/Logger.h"
    "src/Server/Server.cpp"
    "src/Server/Server.h"
    "src/Server/ServerDlg.cpp"
    "src/Server/serverdlg.h"
    "src/Server/ServerDlg.cpp"
    "src/Server/ServerQueues.h"
    "src/Server/StdAfx.h"
    "src/TrajGen/Helix.cpp"
    "src/TrajGen/Helix.h"
    "src/TrajGen/TrajGen.cpp"
    "src/TrajGen/TrajGen.h"
    "resource.h"
    "Server.rc"
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    _AFXDLL
)
