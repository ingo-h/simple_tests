# Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2022-06-20

cmake_minimum_required(VERSION 3.18)
include(../project-header.cmake)

# set the project name and version
project(MP_SOCKETS VERSION 0001
        DESCRIPTION "Inspect and test multiplatform sockets."
        HOMEPAGE_URL "https://github.com/ingo-h")


#################################
# Build the Socket Server       #
#################################
add_executable(socket_server
    ./socket_server.cpp)


#################################
# Build the Unit Tests          #
#################################
add_executable(test_sock
    ./test_sock.cpp)
add_test(NAME ctest_sock COMMAND test_sock)
