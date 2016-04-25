# Recent version of CMake required
cmake_minimum_required(VERSION 3.0)

# Project Name
project(smartstack)

# Variables to compile sources
set(SRCS
    main.cpp
    problem.cpp
    normalStack.cpp
    compressedStack.cpp
    data.cpp
    sign.cpp
    )

set(HEADERS
  problem.hpp
  stack.hpp
  normalStack.hpp
  compressedStack.hpp
  data.hpp
  sign.hpp
    )

# Executable
add_executable(smartstack ${SRCS} ${HEADERS})
