add_executable( blend_test blend_test.cpp )
target_link_libraries( blend_test ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )
