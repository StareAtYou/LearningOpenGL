add_executable( basic_lighting basic_lighting.cpp )
target_link_libraries( basic_lighting ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )
