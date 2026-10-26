################################################################################
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Script (cmake -P) behind the 'misra-analyze' target: dumps the
#              library sources with Cppcheck, runs the MISRA C:2012 addon on the
#              dumps, writes the report and removes the dump files.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# References:
#   - MISRA C:2012 Guidelines for the use of the C language in critical systems
#   - Cppcheck MISRA addon: https://cppcheck.sourceforge.io/misra.php
#   - CMake Documentation: https://cmake.org/documentation/
#
# Last Updated: October 2026
# SPDX-License-Identifier: MIT
#
# Inputs (-D): CPPCHECK, PYTHON, MISRA_SCRIPT, SOURCE_DIR, OUTPUT, CONFIG_DIR and RULE_TEXTS (optional),
#              FAIL_ON_VIOLATION (optional, ON makes any violation a build error)
#
################################################################################

# ==============================================================================
# SOURCE FILES
# ==============================================================================

file(GLOB MISRA_SOURCES ${SOURCE_DIR}/src/*.c)

# Optional folder with the configuration header the sources include
set(MISRA_CONFIG_INCLUDE)
if(CONFIG_DIR)
    set(MISRA_CONFIG_INCLUDE -I ${CONFIG_DIR})
endif()

# ==============================================================================
# DUMP
# ==============================================================================

# Cppcheck writes <file>.dump and <file>.ctu-info next to each source file
execute_process(
    COMMAND ${CPPCHECK}
            --dump
            --inline-suppr          # deviations are marked in the code, see docs/misra.md
            --quiet
            --std=c11
            --suppress=missingIncludeSystem
            -I ${SOURCE_DIR}
            -I ${SOURCE_DIR}/inc
            ${MISRA_CONFIG_INCLUDE}
            ${MISRA_SOURCES}
    RESULT_VARIABLE MISRA_DUMP_RESULT
    OUTPUT_QUIET
    ERROR_QUIET
)

if(NOT MISRA_DUMP_RESULT EQUAL 0)
    message(FATAL_ERROR "Cppcheck --dump failed with code ${MISRA_DUMP_RESULT}")
endif()

# ==============================================================================
# MISRA ADDON
# ==============================================================================

set(MISRA_DUMPS)
foreach(MISRA_SOURCE IN LISTS MISRA_SOURCES)
    list(APPEND MISRA_DUMPS ${MISRA_SOURCE}.dump)
endforeach()

set(MISRA_ARGS)
if(RULE_TEXTS)
    list(APPEND MISRA_ARGS --rule-texts=${RULE_TEXTS})
endif()

execute_process(
    COMMAND ${PYTHON} ${MISRA_SCRIPT} ${MISRA_ARGS} ${MISRA_DUMPS}
    RESULT_VARIABLE MISRA_RESULT
    OUTPUT_VARIABLE MISRA_STDOUT
    ERROR_VARIABLE MISRA_REPORT
)

# ==============================================================================
# CLEANUP & REPORT
# ==============================================================================

foreach(MISRA_SOURCE IN LISTS MISRA_SOURCES)
    file(REMOVE ${MISRA_SOURCE}.dump ${MISRA_SOURCE}.ctu-info)
endforeach()

file(WRITE ${OUTPUT} "${MISRA_REPORT}${MISRA_STDOUT}")
message("${MISRA_REPORT}${MISRA_STDOUT}")

string(REGEX MATCHALL "\\[misra-c2012-[0-9.]+\\]" MISRA_VIOLATIONS "${MISRA_REPORT}")
list(LENGTH MISRA_VIOLATIONS MISRA_VIOLATION_COUNT)
message(STATUS "MISRA C:2012 violations: ${MISRA_VIOLATION_COUNT} (report: ${OUTPUT})")

if(FAIL_ON_VIOLATION AND MISRA_VIOLATION_COUNT GREATER 0)
    message(FATAL_ERROR "MISRA C:2012 violations found; fix them or record a deviation in docs/misra.md")
endif()
