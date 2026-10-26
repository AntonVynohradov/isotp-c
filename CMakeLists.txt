################################################################################
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Top-level CMake file: compiler settings, library, tests and
#              static analysis targets.
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

cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

project(ISOTP
    DESCRIPTION "ISO 15765-2 (ISO-TP) Implementation"
    LANGUAGES C
    )

if(CMAKE_HOST_WIN32)
    set(CMAKE_COLOR_MAKEFILE OFF)
endif()

# ==============================================================================
# COMPILER FLAGS & OPTIONS
# ==============================================================================

# C11 is the newest language revision covered by MISRA C:2012 (Amendment 2)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Compiler warning flags
set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -Wconversion -g")
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -Wpedantic -Wconversion -O2 -DNDEBUG")

# ==============================================================================
# LIBRARY/EXECUTABLE TARGETS
# ==============================================================================

# Unit and integration tests are optional and can be enabled with CMake options
option(ISOTP_UNIT_TESTS "Enable ISOTP unit tests (GoogleTest)" OFF)
option(ISOTP_INTEGRATION_TESTS "Enable ISOTP integration tests (pytest)" OFF)
option(ISOTP_EXAMPLES "Build the host-buildable examples (bare metal simulation, SocketCAN tool)" OFF)

# The static library target is defined in isotp.cmake
include(isotp)

# ==============================================================================
# UNIT TESTS
# ==============================================================================

if(ISOTP_UNIT_TESTS)
    # GTest integration, test discovery and the coverage target
    include(unit_tests)
endif()

# ==============================================================================
# INTEGRATION TESTS
# ==============================================================================

if(ISOTP_INTEGRATION_TESTS)
    # Python codec tests and the C/Python cross-check, run through CTest
    include(integration_tests)
endif()


if(ISOTP_EXAMPLES)
    # The bare-metal example runs against a simulated board as a CTest test
    enable_testing()
    add_subdirectory(examples)
endif()

# ==============================================================================
# STATIC ANALYSIS TARGETS
# ==============================================================================

# Cppcheck static analysis target
# Runs Cppcheck with all checks enabled and writes the result to cppcheck.txt in
# the build directory and to the console. Paths listed in 'cppcheck-excludes.txt'
# are excluded. The target exists only if Cppcheck is found on the system.

find_program(CPPCHECK_EXECUTABLE cppcheck)
if(CPPCHECK_EXECUTABLE)

    set(CPPCHECK_EXCLUDES)
    if(EXISTS "${CMAKE_SOURCE_DIR}/cppcheck-excludes.txt")
        file(STRINGS "${CMAKE_SOURCE_DIR}/cppcheck-excludes.txt" CPPCHECK_EXCLUDE_PATHS)
        foreach(CPPCHECK_EXCLUDE_PATH IN LISTS CPPCHECK_EXCLUDE_PATHS)
            # Absolute paths, because Cppcheck matches them against the absolute source paths below
            if(NOT CPPCHECK_EXCLUDE_PATH STREQUAL "" AND NOT CPPCHECK_EXCLUDE_PATH MATCHES "^#")
                list(APPEND CPPCHECK_EXCLUDES -i "${CMAKE_SOURCE_DIR}/${CPPCHECK_EXCLUDE_PATH}")
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
                --std=c11
                --force
                --template=gcc
                --suppress=missingIncludeSystem
                --suppress=unusedFunction   # public library API is called by integrators, not by the library
                --suppress=checkersReport
                -I ${CMAKE_SOURCE_DIR}
                -I ${CMAKE_SOURCE_DIR}/inc
                ${CPPCHECK_EXCLUDES}
                --output-file=${CMAKE_BINARY_DIR}/cppcheck.txt
                ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_BINARY_DIR}/cppcheck.txt
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running Cppcheck static analysis..."
        VERBATIM
    )

    # Cppcheck MISRA C:2012 addon target
    # Runs the addon directly on Cppcheck dump files (see cmake/misra_check.cmake),
    # because not every Cppcheck package can locate and launch its addons. Rule
    # texts are licensed by MISRA and are not shipped with Cppcheck; pass them with
    # -DISOTP_MISRA_RULE_TEXTS=<file> to get readable messages instead of rule numbers.

    set(ISOTP_MISRA_RULE_TEXTS "" CACHE FILEPATH "Optional MISRA C:2012 rule texts file for Cppcheck")
    option(ISOTP_MISRA_FAIL_ON_VIOLATION "Make 'misra-analyze' fail when the report has violations" ON)

    get_filename_component(CPPCHECK_BIN_DIR ${CPPCHECK_EXECUTABLE} DIRECTORY)
    find_file(ISOTP_MISRA_SCRIPT misra.py
        PATHS ${CPPCHECK_BIN_DIR}/addons
              ${CPPCHECK_BIN_DIR}/../share/cppcheck/addons
              /usr/share/cppcheck/addons
              /usr/local/share/cppcheck/addons
              /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cppcheck/addons   # Debian/Ubuntu packages
        NO_DEFAULT_PATH)
    find_package(Python3 QUIET COMPONENTS Interpreter)

    if(ISOTP_MISRA_SCRIPT AND Python3_Interpreter_FOUND)
        add_custom_target(misra-analyze
            COMMAND ${CMAKE_COMMAND}
                    -DCPPCHECK=${CPPCHECK_EXECUTABLE}
                    -DPYTHON=${Python3_EXECUTABLE}
                    -DMISRA_SCRIPT=${ISOTP_MISRA_SCRIPT}
                    -DRULE_TEXTS=${ISOTP_MISRA_RULE_TEXTS}
                    -DFAIL_ON_VIOLATION=${ISOTP_MISRA_FAIL_ON_VIOLATION}
                    -DSOURCE_DIR=${CMAKE_SOURCE_DIR}
                    -DOUTPUT=${CMAKE_BINARY_DIR}/misra.txt
                    -P ${CMAKE_SOURCE_DIR}/cmake/misra_check.cmake
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running Cppcheck MISRA C:2012 analysis..."
            VERBATIM
        )
    else()
        message(STATUS "Cppcheck misra.py addon or Python 3 not found. Target 'misra-analyze' will be unavailable.")
    endif()
else()
    message(STATUS "Cppcheck not found. Static analysis targets 'cppcheck-analyze' and 'misra-analyze' will be unavailable.")
endif()

# CodeQL analysis target
# Creates a CodeQL database in the build directory, runs the cpp-queries suite
# and writes a SARIF report (codeql-report.sarif). The target exists only if the
# CodeQL CLI is found on the system.

find_program(CODEQL_EXECUTABLE codeql)
if(CODEQL_EXECUTABLE)

    # Note: This target intentionally builds only the core library; examples and
    # tests are excluded to keep the analysis focused. Extend if needed.

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
