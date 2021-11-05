
if(USE_SOCKPP)

    target_link_libraries(${PROJECT_NAME} sockpp)
    
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} ws2_32)
    endif()

endif()


if(USE_CPPFS)

    if(${CMAKE_BUILD_TYPE} STREQUAL Release)
        target_link_libraries(${PROJECT_NAME} cppfs)
    elseif(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        target_link_libraries(${PROJECT_NAME} cppfsd)
    endif()
    
endif()


if(UNIX)

    target_link_libraries(${PROJECT_NAME} pthread)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINKER_LANGUAGE CXX
    )

endif()
