cmake_minimum_required(VERSION 3.16)
project(flusterW)

set(CMAKE_CXX_STANDARD 17)

file(
        GLOB_RECURSE
        source_files
        src/*
)

add_executable(
        flusterW
        #${source_files}
        src/main.cpp
        src/utils/sqlite3.h src/utils/sqlite3.c
        src/utils/base64.cpp src/utils/base64.h src/utils/modp_b64.cpp src/utils/modp_b64.h src/utils/modp_b64_data.h)

#set(CMAKE_CXX_STANDARD 17)
#set(SODIUM_STATIC_LIBRARIES TRUE)
#set(sodium_DIR ${PROJECT_SOURCE_DIR}/contrib)

#find_package(sodium REQUIRED)

#target_link_libraries(flusterW sodium)

#target_include_directories(flusterW PUBLIC ${PROJECT_SOURCE_DIR}/contrib/)
#target_link_libraries(flusterW PUBLIC ${PROJECT_SOURCE_DIR}/../libs/libsodium/libsodium.lib)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/contrib")
include(FindPkgConfig)
find_package(sodium REQUIRED)
include_directories(${SODIUM_INCLURE_DIR})
target_link_libraries(flusterW ${SODIUM_LIBRARY})

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/contrib")
#set(sodium_LIBRARY_RELEASE C:/Users/brieu/Documents/libsodium-stable/libsodium-stable)
#set(sodium_INCLUDE_DIR C:/Users/brieu/Documents/libsodium-stable/libsodium-stable)
#set(sodium_LIBRARY_DEBUG C:/Users/brieu/Documents/libsodium-stable/libsodium-stable)
#find_package(sodium REQUIRED)