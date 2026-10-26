################################################################################
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Extracts the library version from the latest Git tag (vX.Y.Z)
#              and exposes it as ISOTP_VERSION_* compile definitions.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# References:
#   - ISO 15765-2 (ISO-TP) specification: https://www.iso.org/standard/84211.html
#   - MISRA C:2012 Guidelines for the use of the C language in critical systems
#   - CMake Documentation: https://cmake.org/documentation/
#
# Last Updated: October 2026
# SPDX-License-Identifier: MIT
#
################################################################################

# ==============================================================================
# DEFAULT VERSION
# ==============================================================================

# Used when the source tree has no Git metadata or no vX.Y.Z tag yet; keep it in
# sync with the fallback values in isotp.h.
set(ISOTP_DEFAULT_VERSION "2.0.1")

# ==============================================================================
# VERSION EXTRACTION
# ==============================================================================

find_package(Git QUIET)

set(ISOTP_GIT_TAG "")
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
        RESULT_VARIABLE ISOTP_GIT_DESCRIBE_RESULT
        ERROR_VARIABLE ISOTP_GIT_DESCRIBE_ERROR
        OUTPUT_VARIABLE ISOTP_GIT_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )
endif()

# ==============================================================================
# VERSION PARSING
# ==============================================================================

string(REGEX MATCH "^[Vv]([0-9]+)\\.([0-9]+)\\.([0-9]+)$" ISOTP_VERSION_MATCH "${ISOTP_GIT_TAG}")

if(NOT ISOTP_VERSION_MATCH)
    message(WARNING "No vX.Y.Z Git tag found (${ISOTP_GIT_DESCRIBE_ERROR}); "
                    "using default version ${ISOTP_DEFAULT_VERSION}")
    string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)$" ISOTP_VERSION_MATCH "${ISOTP_DEFAULT_VERSION}")
endif()

# ==============================================================================
# VERSION COMPONENTS
# ==============================================================================

set(ISOTP_VERSION_MAJOR "${CMAKE_MATCH_1}")
set(ISOTP_VERSION_MINOR "${CMAKE_MATCH_2}")
set(ISOTP_VERSION_PATCH "${CMAKE_MATCH_3}")

set(ISOTP_VERSION "${ISOTP_VERSION_MAJOR}.${ISOTP_VERSION_MINOR}.${ISOTP_VERSION_PATCH}")

message(STATUS "ISOTP library is version: ${ISOTP_VERSION}")

# ==============================================================================
# CONFIGURE VERSION DEFINITIONS
# ==============================================================================

list(APPEND ISOTP_COMPILE_DEFINITIONS
    ISOTP_VERSION_MAJOR=${ISOTP_VERSION_MAJOR}
    ISOTP_VERSION_MINOR=${ISOTP_VERSION_MINOR}
    ISOTP_VERSION_PATCH=${ISOTP_VERSION_PATCH}
    ISOTP_VERSION="${ISOTP_VERSION}"
)
