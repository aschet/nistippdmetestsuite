cmake_minimum_required(VERSION 3.15)

project(Client)

set(CMAKE_MFC_FLAG 2)

add_executable(${PROJECT_NAME} WIN32)

target_sources(${PROJECT_NAME}
  PRIVATE
    "client/res/Sender.ico"
    "client/res/Sender.rc2"
    "client/coordDlg.cpp"
    "client/coordDlg.h"
    "client/resource.h"
    "client/Sender.cpp"
    "client/Sender.h"
    "client/Sender.rc"
    "client/SenderDlg.cpp"
    "client/SenderDlg.h"
    "client/StdAfx.h"
    "CmdResClasses/IppCmdRes.cpp"
    "CmdResClasses/IppCmdRes.h"
    "comm/socket.cpp"
    "comm/socket.h"
    "respParser/parserRes.cpp"
    "respParser/parserRes.h"
    "strparse/cmdStringParse.cpp"
    "strparse/cmdStringParse.h"
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    _AFXDLL
)
