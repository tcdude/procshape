cmake_minimum_required(VERSION 3.0.0)
project(geomstore VERSION 0.0.1)

include(CTest)
enable_testing()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_FLAGS "-g -Wall")
include(CPack)
