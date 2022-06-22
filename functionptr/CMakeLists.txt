# Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2022-06-22

cmake_minimum_required(VERSION 3.18)
include(../project-header.cmake)

# set the project name and version
project(INSPECT_FUNCTION_POINTER VERSION 0001
        DESCRIPTION "Test how to use function pointer."
        HOMEPAGE_URL "https://github.com/ingo-h")


#################################
# Build the Unit Tests          #
#################################
add_executable(test_functionptr
    ./test_functionptr.cpp)
add_test(NAME ctest_functionptr COMMAND test_functionptr)
