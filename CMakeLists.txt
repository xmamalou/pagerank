cmake_minimum_required(VERSION 3.30)

project(parallel_systems_vol1
        VERSION 0.0.1
        DESCRIPTION "First set of exercises from the 'Parallel Systems' course of the 'Computer Engineering' masters programme of the Informatics and Telecommunications Department of the University of Athens"
        LANGUAGES CXX)

set(APP_NAME exe)
set(SRCFILES 
        src/main.cxx
        src/types.cxx
        src/pagerank.cxx)

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
        set(APP_NAME exe-dbg)
endif()

add_executable(${APP_NAME} ${SRCFILES})
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
target_link_libraries(${APP_NAME} PRIVATE Threads::Threads)
