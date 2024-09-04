add_executable( model_test model_test.cpp )
target_link_libraries( model_test ${ASSIMP_LIBRARY} ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )
