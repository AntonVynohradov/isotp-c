################################################################################
# ISO-TP-C: ISO 15765-2 Protocol Implementation
#
# Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
# Description: Brief description of this CMake file purpose
#
# Author:      Anton Vynohradov
# Email:       avynohradovair@gmail.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# References:
#   - ISO 15765-2 Standard (ISO-TP Protocol)
#   - CERT C Coding Standard: https://wiki.sei.cmu.edu/confluence/display/c/
#   - CMake Documentation: https://cmake.org/documentation/
#
# Last Updated: February 2026
# SPDX-License-Identifier: MIT
#
################################################################################

# ==============================================================================
# CMAKE CONFIGURATION
# ==============================================================================

cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH 

    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ==============================================================================
# INCLUDE DIRECTORIES
# ==============================================================================


# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

project(ISOTP    
    DESCRIPTION "ISO 15765-2 Protocol Implementation"
    LANGUAGES C
    )

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ==============================================================================
# COMPILER FLAGS & OPTIONS
# ==============================================================================

# Compiler warning flags
set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g") #-Wconversion 
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -Wpedantic -O2 -DNDEBUG")

# ==============================================================================
# LIBRARY/EXECUTABLE TARGETS
# ==============================================================================

include(isotp) # The static library target is defined in cmake/isotp.cmake

# ==============================================================================
# DEPENDENCIES
# ==============================================================================

# find_package() calls if needed

# ==============================================================================
# TESTING 
# ==============================================================================

# enable_testing()
# add_subdirectory(tests)

# ==============================================================================
# INSTALLATION
# ==============================================================================

# install(TARGETS PROJECT_NAME ...)
