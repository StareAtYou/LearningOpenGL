cmake_minimum_required( VERSION 3.10 )

include( CompileConfig.cmake )

project( OpenGL_ES3.0_Projects )


find_library( OPENGLES3_LIBRARY  GLESv2  ${THIRDPART_LIBRARY_PATH} )
find_library( EGL_LIBRARY        EGL     ${THIRDPART_LIBRARY_PATH} )
find_library( ASSIMP_LIBRARY     assimp  ${THIRDPART_LIBRARY_PATH} )
find_library( COMMON_LIBRARY     Common  ${THIRDPART_LIBRARY_PATH} )


include_directories( 
    inc
    inc/Common
    inc/External
    inc/Utils
)

link_directories(
    lib
)

subdirs(
    # 3rd/Common
    # 3rd/assimp-5.4.2
    # test
    app
)
		