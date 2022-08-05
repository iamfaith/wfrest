cmake_minimum_required(VERSION 3.6)
project(base)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SRC
    Base64.cc
    ErrorCode.cc
    Compress.cc
    SysInfo.cc     
    Timestamp.cc
    WFJson.cc
)

add_library(${PROJECT_NAME} OBJECT ${SRC})