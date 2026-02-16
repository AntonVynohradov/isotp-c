################################################################################
# ISO-TP-C: ISO 15765-2 Protocol Implementation
#
# Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
# Description: This CMake file configures the build for the ISO-TP-C project.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
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

cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

project(ISOTP
    DESCRIPTION "ISO 15765-2 Protocol Implementation"
    LANGUAGES C
    )

# ==============================================================================
# COMPILER FLAGS & OPTIONS
# ==============================================================================

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Compiler warning flags
set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g") #TODO: -Wconversion
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -Wpedantic -O2 -DNDEBUG")

# ==============================================================================
# LIBRARY/EXECUTABLE TARGETS
# ==============================================================================

# Unit tests are optional and can be enabled with a CMake option
option(ISOTP_UNIT_TESTS "Enable ISO-TP unit tests" OFF)

# The static library target is defined in cmake/isotp.cmake
include(isotp)

# ==============================================================================
# UNIT TESTS
# ==============================================================================

if(ISOTP_UNIT_TESTS)
    # This file contains the configuration for unit tests, including GTest integration and test discovery.
    include( unit_tests)
endif()

# ==============================================================================
# INSTALLATION
# ==============================================================================

# install(TARGETS PROJECT_NAME ...)
