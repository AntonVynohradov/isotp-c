################################################################################
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Library target definition. Can be included directly by an
#              integrating project: include(path/to/isotp.cmake)
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

list(APPEND ISOTP_HEADERS

    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# ==============================================================================
# SOURCE FILES
# ==============================================================================

list(APPEND ISOTP_SOURCES

    ${CMAKE_CURRENT_LIST_DIR}/src/isotp_protocol.c
    ${CMAKE_CURRENT_LIST_DIR}/src/isotp_link.c
)

set_source_files_properties(${ISOTP_SOURCES} PROPERTIES LANGUAGE C)

# Library sources only (ISOTP_SOURCES also receives the test sources below)
set(ISOTP_LIBRARY_SOURCES ${ISOTP_SOURCES})

# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND ISOTP_COMPILE_DEFINITIONS "ISOTP_DEBUG")
endif()

set(ISOTP_LIBRARY_TYPE "STATIC")        # Default to static library, can be overridden by CMake presets
set(ISOTP_LIBRARY_INTERFACE "PUBLIC")   # Interface scope for consumers of the library

# ==============================================================================
# UNIT TESTS
# ==============================================================================

if(ISOTP_UNIT_TESTS)

    list(REMOVE_ITEM ISOTP_SOURCES
        # Add any source files that should be excluded from the main library when unit tests are enabled
    )

    list(APPEND ISOTP_UNIT_TEST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/tests/unit/gtest_isotp_defs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/unit/gtest_isotp_decode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/unit/gtest_isotp_encode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/unit/gtest_isotp_st_min.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/unit/gtest_isotp_link_init.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/unit/gtest_isotp_link_send.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/unit/gtest_isotp_link_receive.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/unit/isotp_test_mocks.cpp
    )

    # 5000-byte buffers make the 32-bit FF_DL paths (messages > 4095 bytes) reachable
    list(APPEND ISOTP_COMPILE_DEFINITIONS
        ISOTP_TX_BUFFER_SIZE=5000U
        ISOTP_RX_BUFFER_SIZE=5000U
    )

    list(APPEND ISOTP_UNIT_TEST_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/tests/unit
    )

    list(APPEND ISOTP_HEADERS ${ISOTP_UNIT_TEST_HEADERS})
    list(APPEND ISOTP_SOURCES ${ISOTP_UNIT_TEST_SOURCES})

    set(ISOTP_LIBRARY_TYPE "INTERFACE")         # Sources are compiled into the test executable (coverage)
    set(ISOTP_LIBRARY_INTERFACE "INTERFACE")    # Consumers (the test executable) link against this target

endif()

# ==============================================================================
# LIBRARY TARGET
# ==============================================================================

set(ISOTP_C "isotp")
add_library(${ISOTP_C} ${ISOTP_LIBRARY_TYPE} ${ISOTP_SOURCES})
target_include_directories(${ISOTP_C} ${ISOTP_LIBRARY_INTERFACE} ${ISOTP_HEADERS})
target_compile_definitions(${ISOTP_C} ${ISOTP_LIBRARY_INTERFACE} ${ISOTP_COMPILE_DEFINITIONS})

if(ISOTP_COMPILE_OPTIONS) # Add any additional compile options if defined
    target_compile_options(${ISOTP_C} ${ISOTP_LIBRARY_INTERFACE} ${ISOTP_COMPILE_OPTIONS})
endif()
