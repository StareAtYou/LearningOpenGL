get_filename_component(EXE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)

include_directories( 
    include
)

aux_source_directory(src SRC_LIST)

add_executable( ${EXE_NAME} ${EXE_NAME}.cpp ${SRC_LIST})
target_link_libraries( ${EXE_NAME} ${ASSIMP_LIBRARY} ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )

add_custom_target(
    run
    COMMAND ${EXE_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    DEPENDS ${EXE_NAME}
    COMMENT "Running ${EXE_NAME} now"  
) 
