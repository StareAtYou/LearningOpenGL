add_executable( fb_test fb_test.cpp )
target_link_libraries( fb_test ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )
