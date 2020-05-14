cmake_minimum_required(VERSION 3.16)
project(flusterW)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_STANDARD 17)

file(
        GLOB_RECURSE
        source_files
        src/*
        contrib/*
)

add_executable(
        flusterW
        ${source_files}
)

#set (Boost_USE_STATIC_LIBS ON)

#set (BOOST_ROOT ${PROJECT_SOURCE_DIR}/../libs/boost_1_66_0/boost/)
#set (BOOST_INCLUDEDIR ${PROJECT_SOURCE_DIR}/../libs/boost_1_66_0/)
#find_package (Boost REQUIRED)