## Please modify this file freely to adapt to the production of other
## executables than upperhull and testrun

# Description of the different builds
# +---------------+--------------+--------------+----------|
# |               | optimization | assert works | stripped |
# +---------------+--------------+--------------+----------|
# |     Debug     |     no       |     yes      |    no    |
# |    Release    |    full      |      no      |   yes    |
# | RelWithDebInfo|    good      |      no      |    no    |
# |   MinSizeRel  |    size      |      no      |   yes    |
# +---------------+--------------+--------------+----------|

# Alias for cmake commands
# alias cmakedebug='cmake $1 -DCMAKE_BUILD_TYPE=DEBUG'
# alias cmakerelease='cmake $1 -DCMAKE_BUILD_TYPE=RELEASE'
# alias cmakerelwithdebinfo='cmake $1 -DCMAKE_BUILD_TYPE=RELWITHDEBINFO'
# alias cmakeminsizerel='cmake $1 -DCMAKE_BUILD_TYPE=MINSIZEREL'

# Example for release (first line only if the directory doesn't exist)
# mkdir build_directory
# cd build_directory
# cmakerelease ..

# To build with Visual Studio (use cmake or cmake.exe or path/cmake.exe depending of your configuration)
# mkdir build_directory
# cd build_directory
# cmake .. -G "Visual Studio 10 Win64"
# cmake.exe .. -G "Visual Studio 11 2012"
# cmake.exe .. -G "Visual Studio 12 2013"
# cmake.exe .. -G "Visual Studio 13 2014"
# cmake.exe .. -G "Visual Studio 14 2015"
# cmake.exe .. -G "Visual Studio 15 2017"


# Recent version of CMake required
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Project Name (change it freely)
project(smartstack)

STRING(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)
# Select the build path in function of the build type: release, debug, ...
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE_LOWER})

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Default build mode is Release with debug information
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

## Configure the different type of build
# Release
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
# Debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
# RelWithDebInfo
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
# MinSizeRel
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    # Basic options for all builds
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wshadow")
	# Might need to be fixed for retrocompatibility or temporary
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++0x-compat -Wno-unused")
endif(CMAKE_COMPILER_IS_GNUCXX)



# Include directories
get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message("inc_dirs = ${inc_dirs}")

# Convex Hulls : 8, 16, 32 or 64 bits and the version with extras
file(GLOB SOURCES "examples/upperhull/upperHull8.cpp")
add_executable(upperhull8 ${SOURCES})
file(GLOB SOURCES "examples/upperhull/upperHull16.cpp")
add_executable(upperhull16 ${SOURCES})
file(GLOB SOURCES "examples/upperhull/upperHull32.cpp")
add_executable(upperhull32 ${SOURCES})
file(GLOB SOURCES "examples/upperhull/upperHull64.cpp")
add_executable(upperhull64 ${SOURCES})
file(GLOB SOURCES "examples/upperhull/upperHullExtras.cpp")
add_executable(upperhullextras ${SOURCES})
file(GLOB SOURCES "examples/upperhull/generateInputUpperHull.cpp")
add_executable(generateInputUpperHull ${SOURCES})


# Test Run : 8, 16, 32 or 64 bits and the version with extras
file(GLOB SOURCES "examples/testrun/testrun8.cpp")
add_executable(testrun8 ${SOURCES})
file(GLOB SOURCES "examples/testrun/testrun16.cpp")
add_executable(testrun16 ${SOURCES})
file(GLOB SOURCES "examples/testrun/testrun32.cpp")
add_executable(testrun32 ${SOURCES})
file(GLOB SOURCES "examples/testrun/testrun64.cpp")
add_executable(testrun64 ${SOURCES})
file(GLOB SOURCES "examples/testrun/testrunExtras.cpp")
add_executable(testrunextras ${SOURCES})
file(GLOB SOURCES "examples/testrun/generateInputTestRun.cpp")
add_executable(generateInputTestRun ${SOURCES})
