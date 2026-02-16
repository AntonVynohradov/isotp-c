################################################################################
# ISO-TP-C: ISO 15765-2 Protocol Implementation
#
# Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
# Description: Brief description of this CMake file purpose
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

set_source_files_properties(${ISOTP_SOURCES} PROPERTIES LANGUAGE C)

# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND COMPILE_DEFINITIONS "ISOTP_DEBUG")
endif()

set(ISOTP_LIBRARY_TYPE "STATIC")        # Default to static library, can be overridden by CMake presets
set(ISOTP_LIBRARY_INTERFACE "PUBLIC")   # Interface target name for consumers of the library

# ==============================================================================
# UNIT TESTS
# ==============================================================================

if(ISOTP_UNIT_TESTS)

    list(REMOVE_ITEM ISOTP_SOURCES
        # Add any source files that should be excluded from the main library when unit tests are enabled
    )

    list(APPEND ISOTP_UNIT_TEST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/tests/gtest_isotp_destroy_link.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/gtest_isotp_init_link.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/gtest_isotp_on_can_message.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/gtest_isotp_poll.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/gtest_isotp_receive.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/gtest_isotp_send.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/gtest_isotp_send_with_id.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/isotp_test_mocks.cpp

    )

    list(APPEND ISOTP_UNIT_TEST_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/tests/
    )

    list(APPEND ISOTP_HEADERS ${ISOTP_UNIT_TEST_HEADERS})
    list(APPEND ISOTP_SOURCES ${ISOTP_UNIT_TEST_SOURCES})

    set(ISOTP_LIBRARY_TYPE "INTERFACE")         # Use an interface library for unit tests to avoid linking issues
    set(ISOTP_LIBRARY_INTERFACE "INTERFACE")    # Interface target name for unit tests, consumers will link against this target

endif()

# ==============================================================================
# LIBRARY TARGET
# ==============================================================================

set(ISOTP_C  "isotp" )
add_library(${ISOTP_C} ${ISOTP_LIBRARY_TYPE} ${ISOTP_SOURCES})
target_include_directories(${ISOTP_C} ${ISOTP_LIBRARY_INTERFACE}  ${ISOTP_HEADERS})
target_compile_definitions(${ISOTP_C} ${ISOTP_LIBRARY_INTERFACE} ${COMPILE_DEFINITIONS})

if(COMPILE_OPTIONS) # Add any additional compile options if defined
    target_compile_options(${ISOTP_C} ${ISOTP_LIBRARY_INTERFACE} ${COMPILE_OPTIONS})
endif()
