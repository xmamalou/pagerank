cmake_minimum_required(VERSION 3.30)

project(parallel_systems_vol1
        VERSION 0.0.1
        DESCRIPTION "First set of exercises from the 'Parallel Systems' course of the 'Computer Engineering' masters programme of the Informatics and Telecommunications Department of the University of Athens"
        LANGUAGES CXX)

set(APP_NAME exe)
set(SRCFILES 
        src/main.cxx
        src/pagerank.cxx)

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
        set(APP_NAME exe-dbg)
        add_executable(${APP_NAME} ${SRCFILES})
else()
        add_executable(${APP_NAME} ${SRCFILES})
endif()
