
if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND copy "$(OutputPath)${PROJECT_NAME}.dll" ${OUT_DIR} /Y
    )
endif()
