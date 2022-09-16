# Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2022-08-05

cmake_minimum_required(VERSION 3.18)
include(../project-header.cmake)

# set the project name and version
project(STRING_LITERALS VERSION 0001
        DESCRIPTION "Test redirection of stderr to a pipe."
        HOMEPAGE_URL "https://github.com/ingo-h")


#################################
# Build the Unit Tests          #
#################################
add_executable(test_pipe
    ./test_pipe.cpp
)
target_include_directories(test_pipe
    PRIVATE
        ${SIMPLE_TESTS_SOURCE_DIR}/../upnplib/gtests/include
        ${SIMPLE_TESTS_SOURCE_DIR}/../upnplib/include
)
target_compile_definitions(test_pipe
    PRIVATE
        DEBUG
)
target_link_libraries(test_pipe
    PRIVATE
        ${SIMPLE_TESTS_SOURCE_DIR}/../upnplib/build/lib/libupnplib_gtest_tools_shared.so
)
add_test(NAME ctest_pipe COMMAND test_pipe)
