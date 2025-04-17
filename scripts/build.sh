#!/usr/bin/bash

cd $(pwd)/build

job_num=1
use_ninja=true
build_type=Release

# argument reader
for i in "$@"; do
    case $i in
    -j=* | --jobs=*)
        num='^[0-9]+$'
        if ! [[ ${i#*=} =~ $num ]]; then
            echo -e "\e[31mERROR! '--jobs' expected a number, but was given something else!\e[0m"
            exit 1
        else 
            job_num=${i#*=}
        fi
        ;;
    -d | --debug)
        build_type=Debug
        ;;
    -m | --no-weeb)
        use_ninja=false
        ;;
    -h | --help | *)
        echo -e "\e[33mParallel Systems Exercise 1 Build Assistant (Bash version) -- Christoforos-Marios Mamaloukas"
        echo "Available options:"
        echo -e "\t* -h, --help: Print this message"
        echo -e "\t* -j=<number>, --jobs=<number>: Amount of processors to use for building"
        echo -e "\t* -d, --debug: Do a Debug build. If not specified, does a Release build"
        echo -e "\t* -m, --no-weeb: Do not use Ninja as a build system\e[0m"
        exit 0
        ;; 
    esac
done

# Deleting cache to allow different build methods to be used
if [ -f "./CMakeCache.txt" ]; then
    echo -e "\e[31mFound cache from previous build! Deleting...\e[0m\n"
    rm "./CMakeCache.txt"
fi

# Building
if [ "$use_ninja" = "true" -a -f "/usr/bin/ninja" ]; then
    echo -e "\e[31mUsing \e[35mNinja\e[31m as build method!\e[32m"
    cmake "-DCMAKE_BUILD_TYPE=$build_type" -G Ninja .. && ninja -j $job_num
else 
    echo -e "\e[31mUsing \e[35mMake\e[31m as build method!\e[32m"
    cmake "-DCMAKE_BUILD_TYPE=$build_type" -G "Unix Makefiles" .. && make -j $job_num
fi

echo -e "\e[0m"

