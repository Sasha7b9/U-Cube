cmake_minimum_required(VERSION 3.0)

project (Demo)

include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yustdafx.h /FIstdafx.h")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

file(GLOB SRC *.cpp *.h *.inl)

source_group(src        FILES ${SRC})

add_executable(${PROJECT_NAME} ${SRC})
