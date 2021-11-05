cmake_minimum_required(VERSION 3.0)

project (Demo)

#set(CMAKE_UNITY_BUILD ON)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_BUILD_TYPE} STREQUAL Release)
include_directories(.
                    ../../Demo/generated/U-Urho/release/include
                    )
elseif(${CMAKE_BUILD_TYPE} STREQUAL Debug)                    
include_directories(.
                    ../../Demo/generated/U-Urho/debug/include
                    )
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

file(GLOB SRC    *.cpp *.h *.inl)

source_group(src FILES ${SRC})

file(GLOB FILES_UNITY ${SRC} )

add_executable(${PROJECT_NAME} ${FILES_UNITY})
