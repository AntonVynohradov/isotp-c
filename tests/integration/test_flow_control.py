#******************************************************************************
# ISO-TP-C: ISO 15765-2 Protocol Implementation
#
# Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
# Description: A high-performance, embedded-grade implementation of the ISO 15765-2 (ISO-TP) protocol in C,
#              designed for automotive and industrial applications. This project focuses on refactoring and optimizing
#              the codebase to achieve maximum efficiency, reliability, and maintainability while adhering to the ISO-TP standard.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
#
# License:     MIT License
#
# Copyright (c) 2026 Anton Vynohradov
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# SPDX-License-Identifier: MIT
#*****************************************************************************


"""! @file test_flow_control.py
@brief Integration tests for flow control handling.
@details Validates protocol results set by FC overflow conditions.
@note FC frames are injected directly via the Python binding.
"""

# =============================================================================
# IMPORTS
# =============================================================================

import pyisotp

# =============================================================================
# FUNCTION PROTOTYPES
# =============================================================================


def _make_payload(size):
    """! @brief Build a deterministic payload of a given size.
    @param size Number of bytes to generate.
    @return Payload bytes.
    @details Uses a simple modulo pattern for repeatability.
    """
    return bytes((idx % 256 for idx in range(size)))


def _fc_frame(flow_status, block_size, st_min):
    """! @brief Build a Flow Control (FC) frame.
    @param flow_status Flow status nibble.
    @param block_size Block size byte.
    @param st_min STmin byte.
    @return CAN frame bytes representing the FC.
    @details Encodes the 0x30 PCI for flow control.
    """
    return bytes([
        0x30 | (flow_status & 0x0F),
        block_size & 0xFF,
        st_min & 0xFF,
    ])


# =============================================================================
# FLOW CONTROL TESTS
# =============================================================================


def test_flow_control_overflow_sets_protocol_result():
    """! @brief Verify overflow FC sets protocol result.
    @details Injects PCI_FLOW_STATUS_OVERFLOW and checks protocol state.
    """
    link = pyisotp.init(0x700, 128, 128)
    payload = _make_payload(20)

    pyisotp.send(link, payload)
    pyisotp.inject_can(link, _fc_frame(0x2, 0, 0))

    assert (
        pyisotp.get_last_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW
    )
