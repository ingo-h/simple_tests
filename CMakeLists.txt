# Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2022-06-21

# Configure and build with:
# cmake -S . -B build
# cmake --build build --config Release

cmake_minimum_required(VERSION 3.18)
include(./project-header.cmake)

# set the project name and version
project(INSPECT_MOCKING VERSION 0001
        DESCRIPTION "Inspect and test how mocking works."
        HOMEPAGE_URL "https://github.com/ingo-h")

enable_testing()

set (BUILD_SHARED_LIBS ON CACHE INTERNAL
    "This option belongs only to GOOGLETEST and build its libraries shared.")  # Implies FORCE

# Set the default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None, Debug, Release, RelWithDebInfo, or Release." FORCE)
    message(STATUS "No build type specified, defaulting to Release.")
endif()


#################################
# Googletest                    #
#################################
# Download, configure and build
include(FetchContent)

message(CHECK_START "Download and configuring GoogleTest")

FetchContent_Declare(
    googletest
    GIT_REPOSITORY    https://github.com/google/googletest.git
    GIT_TAG           origin/main
    GIT_SHALLOW       ON
)
# This caches googletest_SOURCE_DIR, googletest_BINARY_DIR and googletest_POPULATED.
FetchContent_MakeAvailable(googletest)

link_libraries(gtest gmock gtest_main gmock_main
)

message(CHECK_PASS "done")


#################################
# subdirectories                #
#################################
# This caches <subprojectname>_SOURCE_DIR and <subprojectname_BINARY_DIR
# e.g. INSPECT_ACCEPT_SOURCE_DIR and INSPECT_ACCEPT_BINARY_DIR

add_subdirectory(./mp-sockets)
add_subdirectory(./accept)

message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")