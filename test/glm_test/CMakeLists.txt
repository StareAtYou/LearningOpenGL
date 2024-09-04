add_executable( glm_test glm_test.cpp )
target_link_libraries( glm_test ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )
