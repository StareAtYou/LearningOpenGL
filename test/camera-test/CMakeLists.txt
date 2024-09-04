add_executable( camera-test camera-test.cpp )
target_link_libraries( camera-test ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )
