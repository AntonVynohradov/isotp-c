################################################################################
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: CMake configuration for GoogleTest unit tests and coverage
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

enable_language(CXX)

find_package(GTest REQUIRED)

if(NOT GTest_FOUND)
    message(FATAL_ERROR "Google Test not found. Please install Google Test to build unit tests.")
endif()

# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

include(GoogleTest)
include(CTest)

# ==============================================================================
# TESTING
# ==============================================================================

enable_testing()

set(ISOTP_GTEST ${CMAKE_PROJECT_NAME}_gtest)

add_executable(${ISOTP_GTEST} ${ISOTP_SOURCES})

set_target_properties(${ISOTP_GTEST} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    )

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${ISOTP_GTEST} PRIVATE -O0 -g --coverage)
    target_link_options(${ISOTP_GTEST} PRIVATE --coverage)
endif()

target_link_libraries(${ISOTP_GTEST}
                    GTest::gtest
                    GTest::gtest_main
                    isotp
                    )

gtest_discover_tests(${ISOTP_GTEST} PROPERTIES LABELS "unit" DISCOVERY_TIMEOUT 60)

# The library sources must stay warning-free: turn every warning into an error
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set_source_files_properties(${ISOTP_LIBRARY_SOURCES} PROPERTIES
        COMPILE_OPTIONS "-Wall;-Wextra;-Wpedantic;-Wconversion;-Werror")
endif()

# ==============================================================================
# COVERAGE
# ==============================================================================

set(ISOTP_COVERAGE_MIN 100 CACHE STRING "Minimum line and branch coverage (%) required by the 'coverage' target")

find_program(GCOVR_EXECUTABLE gcovr)
if(GCOVR_EXECUTABLE)
    set(COVERAGE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Coverage)
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -L unit
        COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_OUTPUT_DIR}
        COMMAND ${GCOVR_EXECUTABLE}
            -r ${CMAKE_SOURCE_DIR}
            --object-directory ${CMAKE_BINARY_DIR}
            --filter ${CMAKE_SOURCE_DIR}/src
            --exclude ${CMAKE_SOURCE_DIR}/tests
            --lcov ${COVERAGE_OUTPUT_DIR}/coverage.info
            --html-details=${COVERAGE_OUTPUT_DIR}/coverage.html
            --print-summary
            --delete                    # fresh counters for the next run
            --fail-under-line ${ISOTP_COVERAGE_MIN}
            --fail-under-branch ${ISOTP_COVERAGE_MIN}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        USES_TERMINAL
        )
else()
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E echo "gcovr not found. Install gcovr to generate coverage reports."
        )
endif()
