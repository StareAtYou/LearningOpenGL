add_executable( shadow_mapping_test shadow_mapping_test.cpp )
target_link_libraries( shadow_mapping_test ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )

add_executable( DepthVisual DepthVisual.cpp )
target_link_libraries( DepthVisual ${COMMON_LIBRARY} ${OPENGLES3_LIBRARY} ${EGL_LIBRARY} )

