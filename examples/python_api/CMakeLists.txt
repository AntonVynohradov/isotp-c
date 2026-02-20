################################################################################
# ISO-TP-C: ISO 15765-2 Protocol Implementation
#
# Project:     ISOTP_PYTHON_API - The application demonstrating ISO-TP-C usage with Python API
# Description: This CMake file configures the build for the Python API application.
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
    ${CMAKE_CURRENT_SOURCE_DIR}/../../
)

# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

project(pyisotp
    DESCRIPTION "The application demonstrating ISO-TP-C usage with Python API"
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
set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g")
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -Wpedantic -O2 -DNDEBUG")

# ==============================================================================
# PYTHON CONFIGURATION
# ==============================================================================

find_package(Python3 COMPONENTS
    Interpreter
    Development
    REQUIRED
    )

if(NOT Python3_FOUND)
    message(FATAL_ERROR "Python 3 not found. Please install Python 3 to build the Python API.")
endif()

list(APPEND ISOTP_PYTHON_API_HEADERS

    ${CMAKE_CURRENT_LIST_DIR}/inc
)

list(APPEND ISOTP_PYTHON_API_SOURCES

    ${CMAKE_CURRENT_SOURCE_DIR}/pyisotp.c

    ${CMAKE_CURRENT_SOURCE_DIR}/src/can_driver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/isotp_user.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mock_can.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mock_time.c
)

# ==============================================================================
# LIBRARY/EXECUTABLE TARGETS
# ==============================================================================

# Include the main library target for ISO-TP-C
include(isotp)

# ==============================================================================
# APPLICATION
# ==============================================================================

add_library(${CMAKE_PROJECT_NAME} MODULE ${ISOTP_PYTHON_API_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${ISOTP_PYTHON_API_HEADERS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE isotp Python3::Python)

if(NOT WIN32)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE _POSIX_C_SOURCE=200809L)
endif()

if(WIN32)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".pyd")
else()

    set(_py_ext_suffix "${Python3_EXTENSION_SUFFIX}")
    if(NOT _py_ext_suffix)
        set(_py_ext_suffix ".so")
    endif()

    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX "${_py_ext_suffix}")

endif()

# ==============================================================================
# INTEGRATION TESTS
# ==============================================================================

# Integration tests are optional and can be enabled with a CMake option
option(ISOTP_INTEGRATION_TESTS "Enable ISO-TP integration tests for Python API" OFF)

if(ISOTP_INTEGRATION_TESTS)

   enable_testing()

   add_custom_target(integration_tests
    COMMAND ${CMAKE_COMMAND} -E env
    PYTHONPATH=$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
    ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/../../tests/integration
     --color=yes
     --verbose
     --capture=no
     --tb=short
    DEPENDS ${CMAKE_PROJECT_NAME}
    )

endif()