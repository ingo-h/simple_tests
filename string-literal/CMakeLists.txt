# Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2022-06-24

cmake_minimum_required(VERSION 3.18)
include(../project-header.cmake)

# set the project name and version
project(STRING_LITERALS VERSION 0001
        DESCRIPTION "Inspect and test to mock string literals."
        HOMEPAGE_URL "https://github.com/ingo-h")


#################################
# Build the Unit Tests          #
#################################
add_executable(test_string_literal
    ./test_string_literal.cpp)
add_test(NAME ctest_string_literal COMMAND test_string_literal)
