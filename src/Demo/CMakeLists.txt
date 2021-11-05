cmake_minimum_required(VERSION 3.0)

project (Demo)

set(CMAKE_UNITY_BUILD ON)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(TYPE_BUILD release)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(TYPE_BUILD debug)
endif()

include_directories(../../Demo/generated/U-Urho/${TYPE_BUILD}/include
                    ../../Demo/generated/U-Urho/${TYPE_BUILD}/include/Urho3D/ThirdParty)
link_directories(../../Demo/generated/U-Urho/${TYPE_BUILD}/lib)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS  /STACK:100000000")

file(GLOB SRC    *.cpp *.h *.inl)

source_group(src FILES ${SRC})

file(GLOB FILES_UNITY ${SRC} )

add_executable(${PROJECT_NAME} ${FILES_UNITY})

if(WIN32)
    if(${CMAKE_BUILD_TYPE} STREQUAL Release)
        target_link_libraries(${PROJECT_NAME} Urho3D)
    else()
        target_link_libraries(${PROJECT_NAME} Urho3D_d)
    endif()
else()
    target_link_libraries(${PROJECT_NAME} Urho3D)
endif()