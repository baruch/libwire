cmake_minimum_required (VERSION 3.0)
project (LIBWIRE)

add_subdirectory (src)

if ( ${CMAKE_PROJECT_NAME} STREQUAL "LIBWIRE" )
    add_subdirectory (test)
    MESSAGE(STATUS "top level project, compiling tests")
else()
    MESSAGE(STATUS "tests will not be compiled")
endif()
