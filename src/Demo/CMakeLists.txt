cmake_minimum_required(VERSION 3.0)

project (Demo)

set(CMAKE_UNITY_BUILD ON)

include_directories(.
if(${CMAKE_BUILD_TYPE} STREQUAL Release)
                    ../../Demo/generated/U-Urho/release/include
else()
                    ../../Demo/generated/U-Urho/debug/include
endif()
                    )

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

file(GLOB SRC    *.cpp *.h *.inl)

source_group(src FILES ${SRC})

file(GLOB FILES_UNITY ${SRC} )

add_executable(${PROJECT_NAME} ${FILES_UNITY})
