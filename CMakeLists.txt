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

cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH

    ${CMAKE_CURRENT_SOURCE_DIR}
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
set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g") #-Wconversion
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

    # == Google Test Integration ==

    find_package(GTest REQUIRED)
    include(GoogleTest)
    include(CTest)

    enable_language(CXX)
    enable_testing()

    # =============================

    set(ISOTP_GTEST ${CMAKE_PROJECT_NAME}_gtest)

    add_executable(${ISOTP_GTEST} ${ISOTP_SOURCES} )

    set_target_properties(${ISOTP_GTEST} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${ISOTP_GTEST} PRIVATE -O0 -g --coverage)
        target_link_options(${ISOTP_GTEST} PRIVATE --coverage)
    endif()

    target_link_libraries(${ISOTP_GTEST}
                        GTest::GTest
                        GTest::Main
                        isotp
                        )

    gtest_discover_tests(${ISOTP_GTEST} PROPERTIES LABELS "unit")


    find_program(GCOVR_EXECUTABLE gcovr)
    if(GCOVR_EXECUTABLE)
        set(COVERAGE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Coverage)
        add_custom_target(coverage
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_OUTPUT_DIR}
            COMMAND ${GCOVR_EXECUTABLE}
                -r ${CMAKE_SOURCE_DIR}
                --object-directory ${CMAKE_BINARY_DIR}
                --filter ${CMAKE_SOURCE_DIR}/src
                --exclude ${CMAKE_SOURCE_DIR}/tests
                --lcov ${COVERAGE_OUTPUT_DIR}/coverage.info
                --html-details=${COVERAGE_OUTPUT_DIR}/coverage.html
                --print-summary
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            USES_TERMINAL
            )
    else()
        add_custom_target(coverage
            COMMAND ${CMAKE_COMMAND} -E echo "gcovr not found. Install gcovr to generate coverage reports."
            )
    endif()

endif()

# ==============================================================================
# INSTALLATION
# ==============================================================================

# install(TARGETS PROJECT_NAME ...)
