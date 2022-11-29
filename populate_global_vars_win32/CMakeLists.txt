# Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2022-11-29

cmake_minimum_required(VERSION 3.18)
include(../project-header.cmake)

# set the project name and version
project(POPULATE_GLOBAL_VARS_WIN32 VERSION 0001
        DESCRIPTION "Test populate global variables on dependent libraries WIN32."
        HOMEPAGE_URL "https://github.com/ingo-h")


#################################
# Build the Unit Tests          #
#################################
if(WIN32)

    add_library(library_a_shared SHARED
    #----------------------------------
        ./library_a.cpp
    )

    add_library(library_b_shared SHARED
    #----------------------------------
        ./library_b.cpp
    )
    target_link_libraries(library_b_shared
        PUBLIC
            library_a_shared
    )

    add_executable(consumer_shared
    #-----------------------------
        ./consumer.cpp
    )
    target_link_libraries(consumer_shared
        PRIVATE
            library_b_shared
    )

else()
    message(FATAL_ERROR "Can only be built on Microsoft Windows.")
endif()
