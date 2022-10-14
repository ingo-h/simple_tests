# Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2022-10-14

cmake_minimum_required(VERSION 3.18)
include(../project-header.cmake)

# set the project name and version
project(LINK_DEPENDENT_LIBRARY VERSION 0001
        DESCRIPTION "Test dependency of linked libraries."
        HOMEPAGE_URL "https://github.com/ingo-h")


# We have visibility support as described at https://gcc.gnu.org/wiki/Visibility
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)


#################################
# Build the Unit Tests          #
#################################
add_library(library_a_shared SHARED
#----------------------------------
    ./library_a.cpp
)
target_compile_definitions(library_a_shared
    PRIVATE
        UPNPLIB_SHARED
        UPNPLIB_EXPORTS
)

add_library(library_b_shared SHARED
#----------------------------------
    ./library_b.cpp
)
target_compile_definitions(library_b_shared
    PRIVATE
        UPNPLIB_SHARED
        UPNPLIB_EXPORTS
)
target_link_libraries(library_b_shared
    PUBLIC
        library_a_shared
)

add_executable(consumer_shared
#-----------------------------
    ./consumer.cpp
)
target_compile_definitions(consumer_shared
    PRIVATE
        UPNPLIB_SHARED
)
target_link_libraries(consumer_shared
    PRIVATE
        library_b_shared
)
