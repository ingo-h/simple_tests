# Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2022-06-21

cmake_minimum_required(VERSION 3.18)
include(../project-header.cmake)

# set the project name and version
project(INSPECT_ACCEPT VERSION 0001
        DESCRIPTION "Inspect and test accept system function."
        HOMEPAGE_URL "https://github.com/ingo-h")


#################################
# Build the Unit Tests          #
#################################
add_executable(test_accept
    ./test_accept.cpp)
add_test(NAME ctest_accept COMMAND test_accept)
