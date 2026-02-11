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
    RESULT_VARIABLE ISOTP_GIT_DESCRIBE_RESULT
    ERROR_VARIABLE ISOTP_GIT_DESCRIBE_ERROR
    OUTPUT_VARIABLE ISOTP_GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT ISOTP_GIT_DESCRIBE_RESULT EQUAL 0 OR ISOTP_GIT_TAG STREQUAL "")
    message(FATAL_ERROR "git describe failed or returned no tag; Error: ${ISOTP_GIT_DESCRIBE_ERROR}")    
endif()

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
