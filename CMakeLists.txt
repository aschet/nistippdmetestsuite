cmake_minimum_required(VERSION 3.15)

project(NISTIPPDMEtestSuite)

add_subdirectory("standAloneTestSuiteComponents")
if(WIN32)
  add_subdirectory("testSuiteComponents")
endif()
