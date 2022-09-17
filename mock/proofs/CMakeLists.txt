# Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2022-09-18

cmake_minimum_required(VERSION 3.18)
include(../../project-header.cmake)

# set the project name and version
project(PROOFS VERSION 0001
        DESCRIPTION "Proof some constructs."
        HOMEPAGE_URL "https://github.com/ingo-h")


#################################
# Build the Unit Tests          #
#################################

# Some pointer arithmetic to address side effects
#------------------------------------------------
add_executable(test_proof1
    ./proof1.cpp)
add_test(NAME ctest_proof1 COMMAND test_proof1)


# Simple dependency injection
#----------------------------
add_executable(test_proof2-work
    ./proof2.cpp)
add_test(NAME ctest_proof2-work COMMAND test_proof2-work)

add_executable(test_proof2-mock
    ./proof2.cpp)
target_compile_definitions(test_proof2-mock
    PRIVATE
        MOCK)
add_test(NAME ctest_proof2-mock COMMAND test_proof2-mock)


# Dependency injection with mocking side effect
#----------------------------------------------
add_executable(test_proof3-work
    ./proof3.cpp)
add_test(NAME ctest_proof3-work COMMAND test_proof3-work)

add_executable(test_proof3-mock
    ./proof3.cpp)
target_compile_definitions(test_proof3-mock
    PRIVATE
        MOCK)
add_test(NAME ctest_proof3-mock COMMAND test_proof3-mock)


# Using global pointer instead of dependency injection
#-----------------------------------------------------
add_executable(test_proof4-work
    ./proof4.cpp)
add_test(NAME ctest_proof4-work COMMAND test_proof4-work)

add_executable(test_proof4-mock
    ./proof4.cpp)
target_compile_definitions(test_proof4-mock
    PRIVATE
        MOCK)
add_test(NAME ctest_proof4-mock COMMAND test_proof4-mock)


# Use dependency injection with a static member variable.
# This is the final combination of the two proofs before.
#--------------------------------------------------------
add_executable(test_proof5-work
    ./proof5.cpp)
target_include_directories(test_proof5-work
    PRIVATE
        ${UPnPlib_SOURCE_DIR}/include)
add_test(NAME ctest_proof5-work COMMAND test_proof5-work)

add_executable(test_proof5-mock
    ./proof5.cpp)
target_include_directories(test_proof5-mock
    PRIVATE
        ${UPnPlib_SOURCE_DIR}/include)
target_compile_definitions(test_proof5-mock
    PRIVATE
        MOCK)
add_test(NAME ctest_proof5-mock COMMAND test_proof5-mock)
