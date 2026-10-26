################################################################################
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Zephyr application: the bare-metal ECU on the Zephyr CAN API.
#              west build -b <board> examples/zephyr
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#
################################################################################

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(isotp_zephyr_ecu LANGUAGES C)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../isotp.cmake)

target_link_libraries(isotp PUBLIC zephyr_interface)

target_sources(app PRIVATE src/main.c)
target_link_libraries(app PRIVATE isotp)
