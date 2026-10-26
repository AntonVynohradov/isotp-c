################################################################################
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Host-buildable examples (ISOTP_EXAMPLES=ON). The Zephyr example
#              is built with west, not from here.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#
################################################################################

# Bare-metal ECU on the simulated board: a self-checking CTest test
add_executable(isotp_bare_metal_sim
    bare_metal/main.c
    bare_metal/board_host.c
)
target_link_libraries(isotp_bare_metal_sim PRIVATE isotp)

add_test(NAME example_bare_metal COMMAND isotp_bare_metal_sim)
set_tests_properties(example_bare_metal PROPERTIES LABELS "example")

# SocketCAN request tool (Linux only)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_executable(isotp_request
        linux_socketcan/main.c
    )
    target_link_libraries(isotp_request PRIVATE isotp)
endif()
