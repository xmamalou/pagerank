cmake_minimum_required(VERSION 3.30)

project(parallel_systems_vol1
        VERSION 0.0.1
        DESCRIPTION "First set of exercises from the 'Parallel Systems' course of the 'Computer Engineering' masters programme of the Informatics and Telecommunications Department of the University of Athens"
        LANGUAGES C)

set(APP_NAME exe)
set(SRCFILES 
        src/main.c 
        src/pagerank.c
        src/utility.c)

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
        set(APP_NAME exe-dbg)
        add_executable(${APP_NAME} ${SRCFILES})
else()
        add_executable(${APP_NAME} ${SRCFILES})
endif()

target_compile_options(${APP_NAME} PRIVATE -D_POSIX_C_SOURCE=200809L)