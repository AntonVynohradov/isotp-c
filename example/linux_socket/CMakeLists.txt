################################################################################
# ISO-TP-C: ISO 15765-2 Protocol Implementation
#
# Project:     LinuxSocketExample - Example application demonstrating ISO-TP-C usage with Linux SocketCAN
# Description: This CMake file configures the build for the LinuxSocketExample project.
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
    ${CMAKE_CURRENT_SOURCE_DIR}/../../
)

# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

project(LinuxSocketExample
    DESCRIPTION "Example application demonstrating ISO-TP-C usage with Linux SocketCAN"
    LANGUAGES C
    )

# ==============================================================================
# COMPILER FLAGS & OPTIONS
# ==============================================================================

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_GENERATOR "Unix Makefiles") # Force Unix Makefiles generator for better compatibility with Linux build environments

# Compiler warning flags
set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g")
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -Wpedantic -O2 -DNDEBUG")

# ==============================================================================
# LIBRARY/EXECUTABLE TARGETS
# ==============================================================================

# Include the main library target for ISO-TP-C
include(isotp)

# ==============================================================================
# EXAMPLE APPLICATION
# ==============================================================================

add_executable(${CMAKE_PROJECT_NAME} linux_socket.c)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE isotp )
