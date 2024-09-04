add_executable( env_mapping_test env_mapping_test.cpp )
target_link_libraries( env_mapping_test ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )
