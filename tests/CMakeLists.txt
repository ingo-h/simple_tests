# Copyright (C) 2023+ GPL 3 and higher by Ingo Höft, <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2023-03-28

cmake_minimum_required(VERSION 3.18)
include(../project-header.cmake)

# set the project name and version
project(GTEST_BUG VERSION 0001
        DESCRIPTION "Sample test to show a bug with gtest."
        HOMEPAGE_URL "https://github.com/ingo-h")


    add_executable(gtest_bug
    #-----------------------
        ./test_gtest_bug.cpp
    )
