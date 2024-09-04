
# Compiler Version <x86_64-14.1.0-release-posix-seh-msvcrt-rt_v12-rev0>
set(CMAKE_C_COMPILER    gcc)
set(CMAKE_CXX_COMPILER  g++)

set(THIRDPART_LIBRARY_PATH  ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_SOURCE_DIR}/bin)
# file(COPY ${CMAKE_SOURCE_DIR}/lib/ DESTINATION ${CMAKE_SOURCE_DIR}/out)


# if (${COMMON_LIBRARY} STREQUAL "COMMON_LIBRARY-NOTFOUND")
#     message (STATUS "Common library will be generated during compilation, because required Common library failed")
#     include_directories( inc/Common )
#     subdirs( 3rd/Common )
#     set( COMMON_LIBRARY Common )
# else()
#     message (STATUS "Common library found in ${COMMON_LIBRARY}")
# endif()