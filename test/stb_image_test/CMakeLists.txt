add_executable( stb_image_test stb_image_test.cpp )
target_link_libraries( stb_image_test ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )
