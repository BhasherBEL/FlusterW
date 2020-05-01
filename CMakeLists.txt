cmake_minimum_required(VERSION 3.16)
project(flusterW)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

file(
        GLOB_RECURSE
        source_files
        src/*
)

add_executable(
        flusterW
        ${source_files} src/utils/rand.cpp src/utils/rand.h)