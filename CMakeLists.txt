# Recent version of CMake required
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Project Name
project(smartstack)

# Select the build path in function of the build type: release, debug, ...
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

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

# Basic options for all builds
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Weffc++ -Wshadow -ansi")

#Bring the headers into the project
include_directories(${PROJECT_SOURCE_DIR}/include)

#Bring the sources
file(GLOB SOURCES "src/*.cpp")

# Test directories
get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message("inc_dirs = ${inc_dirs}")

# Executable
add_executable(smartstack ${SOURCES})


# Description of the different builds
# +---------------+--------------+--------------+----------|
# |               | optimization | assert works | stripped |
# +---------------+--------------+--------------+----------|
# |     Debug     |     no       |     yes      |    no    |
# |    Release    |    full      |      no      |   yes    |
# | RelWithDebInfo|    good      |      no      |    no    |
# |   MinSizeRel  |    size      |      no      |   yes    |
# +---------------+--------------+--------------+----------|
