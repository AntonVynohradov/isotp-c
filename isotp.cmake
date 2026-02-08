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

list(APPEND CMAKE_MODULE_PATH 

    ${CMAKE_CURRENT_LIST_DIR}/cmake
)

# ==============================================================================
# VERSION EXTRACTION
# ==============================================================================

include(version) # Include the version extraction and parsing logic

# ==============================================================================
# INCLUDE DIRECTORIES
# ==============================================================================

include_directories( ${CMAKE_CURRENT_LIST_DIR} )

list(APPEND ISOTP_HEADERS

    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# ==============================================================================
# SOURCE FILES
# ==============================================================================

list(APPEND ISOTP_SOURCES

    ${CMAKE_CURRENT_LIST_DIR}/src/isotp.c
)

set_source_files_properties(ISOTP_SOURCES LANGUAGE C)

# ==============================================================================
# STATIC LIBRARY TARGET
# ==============================================================================

set(ISOTP_C  "isotp" ) 
add_library(${ISOTP_C} STATIC ${ISOTP_SOURCES})
target_include_directories(${ISOTP_C} PRIVATE ${ISOTP_HEADERS})
target_compile_definitions(${ISOTP_C} PRIVATE ${COMPILE_DEFINITIONS})
target_compile_options(${ISOTP_C} PRIVATE ${COMPILE_OPTIONS})