cmake_minimum_required (VERSION 3.0)
project (LIBWIRE)

if ( ${UNIX} )
        execute_process(COMMAND bash -c "${CMAKE_C_COMPILER} -v 2>&1 | grep Target: | cut -f2 -d' ' | cut -f1 -d- "
                        OUTPUT_VARIABLE ARCH
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        MESSAGE(STATUS "ARCH: ${ARCH}")
        if ( ${ARCH} STREQUAL "x86_64")
                MESSAGE(STATUS "Using internal (libcoro inspired) for coroutine implementation (faster)")
                set(USE_CORO 0)
        else()
                MESSAGE(STATUS "Using libcoro for non x86-64 arch")
                set(USE_CORO 1)
        endif()
else()
        MESSAGE(STATUS "Using libcoro for coroutine implementation")
        set(USE_CORO 1)
endif()

add_subdirectory (src)

if ( ${CMAKE_PROJECT_NAME} STREQUAL "LIBWIRE" )
    add_subdirectory (test)
    MESSAGE(STATUS "top level project, compiling tests")
else()
    MESSAGE(STATUS "tests will not be compiled")
endif()
