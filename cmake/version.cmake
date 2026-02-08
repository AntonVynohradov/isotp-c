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
# VERSION EXTRACTION
# ==============================================================================

execute_process(
    COMMAND git describe --tags --abbrev=0
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE ISOTP_GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# ==============================================================================
# VERSION PARSING
# ==============================================================================

string(REGEX MATCH "^[Vv]([0-9]+)\\.([0-9]+)\\.([0-9]+)$" ISOTP_VERSION_MATCH "${ISOTP_GIT_TAG}")

# ==============================================================================
# VERSION COMPONENTS
# ==============================================================================

if(ISOTP_VERSION_MATCH)
    
    set(ISOTP_VERSION_MAJOR "${CMAKE_MATCH_1}")
    set(ISOTP_VERSION_MINOR "${CMAKE_MATCH_2}")
    set(ISOTP_VERSION_PATCH "${CMAKE_MATCH_3}")

    set(ISOTP_VERSION "${ISOTP_VERSION_MAJOR}.${ISOTP_VERSION_MINOR}.${ISOTP_VERSION_PATCH}")
    
    message( STATUS "ISO-TP library is version: ${ISOTP_VERSION}" )

else()

    message(FATAL_ERROR "Failed to extract version from git tag: ${ISOTP_GIT_TAG}. Expected format: vMAJOR.MINOR.PATCH")
    
endif()


# ==============================================================================
# CONFIGURE VERSION HEADER
# ==============================================================================

list(APPEND COMPILE_DEFINITIONS
    ISOTP_VERSION_MAJOR=${ISOTP_VERSION_MAJOR}
    ISOTP_VERSION_MINOR=${ISOTP_VERSION_MINOR}
    ISOTP_VERSION_PATCH=${ISOTP_VERSION_PATCH}
    ISOTP_VERSION="${ISOTP_VERSION}"
)
