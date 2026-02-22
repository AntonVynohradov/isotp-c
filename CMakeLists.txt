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

if(CMAKE_HOST_WIN32)
    set(CMAKE_COLOR_MAKEFILE OFF)
endif()

# ==============================================================================
# COMPILER FLAGS & OPTIONS
# ==============================================================================

# Set C standard
set(CMAKE_C_STANDARD 17)
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
# STATIC ANALYSIS TARGETS
# ==============================================================================

# Cppcheck static analysis target
# This target runs Cppcheck with a comprehensive set of checks and outputs the results to a file and the console.
# It also supports excluding specific directories or files by listing them in a 'cppcheck-excludes.txt' file in the project root.
# The Cppcheck target is only available if Cppcheck is found on the system. If not, a message is printed to the console and the target is not created.

find_program(CPPCHECK_EXECUTABLE cppcheck)
if(CPPCHECK_EXECUTABLE)

    set(CPPCHECK_EXCLUDES)
    if(EXISTS "${CMAKE_SOURCE_DIR}/cppcheck-excludes.txt")
        file(STRINGS "${CMAKE_SOURCE_DIR}/cppcheck-excludes.txt" CPPCHECK_EXCLUDE_PATHS)
        foreach(CPPCHECK_EXCLUDE_PATH IN LISTS CPPCHECK_EXCLUDE_PATHS)
            if(NOT CPPCHECK_EXCLUDE_PATH STREQUAL "")
                list(APPEND CPPCHECK_EXCLUDES -i "${CPPCHECK_EXCLUDE_PATH}")
            endif()
        endforeach()
    endif()

    add_custom_target(cppcheck-analyze
        COMMAND ${CPPCHECK_EXECUTABLE}
                --enable=all
                --check-level=exhaustive
                --inconclusive
                --verbose
                --quiet
                --std=c17
                --force
                --template=gcc
                ${CPPCHECK_EXCLUDES}
                --output-file=${CMAKE_BINARY_DIR}/cppcheck.txt
                ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_BINARY_DIR}/cppcheck.txt
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running Cppcheck static analysis..."
        VERBATIM
    )
else()
    message(STATUS "Cppcheck not found. Static analysis target 'cppcheck' will be unavailable.")
endif()

# CodeQL analysis target - assumes a CodeQL database has been created with the name 'codeql-db' in the build directory
# This target runs CodeQL analysis using the cpp-queries suite and outputs a SARIF report (codeql-report.sarif). It is only available if the CodeQL CLI is found on the system.
# The CodeQL target is designed to be flexible and can be easily modified to include additional query suites or output formats as needed.
# It also assumes that the CodeQL database has been properly set up with the necessary source files and build configuration for accurate analysis.
# To create the CodeQL database, you can use the following command in the build directory:
#   codeql database create codeql-db --language=c --overwrite --source-root=../ --command "cmake --build . --target codeql-build"

find_program(CODEQL_EXECUTABLE codeql)
if(CODEQL_EXECUTABLE)

    # Note: This target intentionally builds only the core library by default; examples/tests
    #are excluded to keep the analysis focused and avoid extra dependencies. Extend if needed.

    if(WIN32)
        set(CODEQL_BUILD_SCRIPT "${CMAKE_BINARY_DIR}/codeql_build.cmd")
        file(WRITE "${CODEQL_BUILD_SCRIPT}" "\"${CMAKE_COMMAND}\" --build \"${CMAKE_BINARY_DIR}\" --target isotp\r\n")
        set(CODEQL_BUILD_COMMAND "${CODEQL_BUILD_SCRIPT}")
    else()
        set(CODEQL_BUILD_COMMAND "${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target isotp")
    endif()

    add_custom_target(codeql-analyze
        COMMAND ${CODEQL_EXECUTABLE} database create
            ${CMAKE_BINARY_DIR}/codeql-db
            --language=c
            --overwrite
            --source-root=${CMAKE_SOURCE_DIR}
            --command "${CODEQL_BUILD_COMMAND}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Creating CodeQL database..."

        COMMAND ${CODEQL_EXECUTABLE} database analyze
                ${CMAKE_BINARY_DIR}/codeql-db
                codeql/cpp-queries
                --format=sarif-latest
                --output=${CMAKE_BINARY_DIR}/codeql-report.sarif
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running CodeQL analysis..."
        VERBATIM
    )
else()
    message(STATUS "CodeQL not found. Static analysis target 'codeql-analyze' will be unavailable.")
endif()

# ==============================================================================
# INSTALLATION
# ==============================================================================

# install(TARGETS PROJECT_NAME ...)
