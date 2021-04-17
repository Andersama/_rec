# CMakeList.txt : CMake project for _rec, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#https://github.com/cycfi/elements
if (NOT ELEMENTS_ROOT)
   message(FATAL_ERROR "ELEMENTS_ROOT is not set")
endif()
#https://github.com/cycfi/q
if (NOT Q_ROOT)
    message(FATAL_ERROR "Q_ROOT is not set")
endif()

if (NOT FMT_ROOT)
    message(FATAL_ERROR "FMT_ROOT is not set")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
      OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0")
endif()

set(CMAKE_CXX_STANDARD 17)

# Make sure ELEMENTS_ROOT is an absolute path to add to the CMake module path
get_filename_component(ELEMENTS_ROOT "${ELEMENTS_ROOT}" ABSOLUTE)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${ELEMENTS_ROOT}/cmake")

include(ElementsConfigCommon)
set(ELEMENTS_BUILD_EXAMPLES OFF)
add_subdirectory(${ELEMENTS_ROOT} elements)

set(Q_BUILD_EXAMPLES OFF)
set(Q_BUILD_TEST OFF)
add_subdirectory(${Q_ROOT} q)

add_subdirectory(${FMT_ROOT} fmt)

set(ELEMENTS_APP_PROJECT "_rec")
set(ELEMENTS_APP_TITLE "_rec")
set(ELEMENTS_APP_COPYRIGHT "Copyright (c) 2021 Alexander Anderson")
set(ELEMENTS_APP_ID "com.andersama._rec")
set(ELEMENTS_APP_VERSION "1.0")

# Add source to this project's executable.
# add_executable (_rec "_rec.cpp" "_rec.h")

set(ELEMENTS_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/_rec.cpp
    )

set(ELEMENTS_APP_INCLUDE_DIRECTORIES
    #"${FMT_ROOT}/include"
    )

#link q libraries
set(ELEMENTS_APP_DEPENDENCIES
    libq
    libqio
    cycfi::infra
    fmt::fmt-header-only
)
# TODO: Add tests and install targets if needed.
include(ElementsConfigApp)

#target_link_libraries("_rec" libq libqio cycfi::infra)