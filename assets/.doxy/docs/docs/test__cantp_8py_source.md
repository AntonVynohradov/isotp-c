

# File test\_cantp.py

[**File List**](files.md) **>** [**integration**](dir_9ddfaab4002c4e8cb6c25cc17bdef75b.md) **>** [**test\_cantp.py**](test__cantp_8py.md)

[Go to the documentation of this file](test__cantp_8py.md)


```Python
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


"""! @file test_cantp.py
@brief Integration tests for CAN TP and ISO-TP behavior.
@details Exercises FC handling, timeouts, and unexpected PDU handling.
@note Some tests rely on mock drop/disable helpers from the Python binding.
"""

# =============================================================================
# IMPORTS
# =============================================================================

import pyisotp

# =============================================================================
# FUNCTION PROTOTYPES
# =============================================================================


def _poll_until_receive(link, payload_size, steps, advance_ms=0):
    """! @brief Poll the link until a response is received or steps are exhausted.
    @param link ISO-TP link handle.
    @param payload_size Expected payload size for receive.
    @param steps Maximum number of poll iterations.
    @param advance_ms Optional time advance per step in milliseconds.
    @return Received payload bytes or None when nothing is received.
    @details Optionally advances the mock time between polls.
    """
    resp = None
    for _ in range(steps):
        if advance_ms:
            pyisotp.time_advance(advance_ms)
        pyisotp.poll(link)
        resp = pyisotp.receive(link, payload_size)
        if resp is not None:
            break
    return resp


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
# CAN TP / ISO-TP BEHAVIOR TESTS
# =============================================================================


def test_wait_frame_then_continue():
    """! @brief Verify receiver resumes after WAIT then CONTINUE FC.
    @details WAIT should pause transmission until CONTINUE arrives.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    payload = _make_payload(20)

    pyisotp.send(link, payload)
    pyisotp.inject_can(link, _fc_frame(0x1, 1, 0))
    pyisotp.inject_can(link, _fc_frame(0x0, 0, 0))

    resp = _poll_until_receive(link, 1024, 10)
    assert resp == payload


def test_wait_frame_overrun():
    """! @brief Verify WAIT frame overrun is reported.
    @details Repeated WAIT frames exceed the limit and trigger WFT_OVRN.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    payload = _make_payload(20)

    pyisotp.send(link, payload)
    pyisotp.inject_can(link, _fc_frame(0x1, 1, 0))
    pyisotp.inject_can(link, _fc_frame(0x1, 1, 0))

    assert (
        pyisotp.get_last_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_WFT_OVRN
    )


def test_cantp_timeout_n_bs():
    """! @brief Verify N_Bs timeout is detected.
    @details Disables FC to force a sender block-size timeout.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    pyisotp.set_timeouts(link, 30, 10)
    pyisotp.mock_disable_fc(True)

    pyisotp.send(link, _make_payload(30))
    for _ in range(31):
        pyisotp.time_advance(1)
        pyisotp.poll(link)

    assert pyisotp.get_last_protocol_result(link) == pyisotp.ISOTP_PROTOCOL_RESULT_TIMEOUT_BS


def test_cantp_timeout_n_cr():
    """! @brief Verify N_Cr timeout is detected.
    @details Drops incoming frames to force a receiver timeout.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    pyisotp.set_timeouts(link, 30, 10)

    pyisotp.send(link, _make_payload(20))
    pyisotp.poll(link)

    pyisotp.mock_enable_drop(True)
    for _ in range(11):
        pyisotp.time_advance(1)
        pyisotp.poll(link)

    assert (
        pyisotp.get_last_receive_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_TIMEOUT_CR
    )
    pyisotp.mock_enable_drop(False)


def test_cantp_stmin_and_bs():
    """! @brief Verify STmin and block size pacing.
    @details Advances time to satisfy STmin between frames.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    pyisotp.set_fc_params(link, 1, 5)

    payload = _make_payload(25)
    pyisotp.send(link, payload)

    resp = _poll_until_receive(link, 1024, 20, advance_ms=1)
    assert resp == payload


def test_cantp_unexpected_pdu():
    """! @brief Verify unexpected PDU is reported.
    @details Injects a CF when receiver is idle.
    """
    link = pyisotp.init(0x700, 512, 512)
    pyisotp.inject_can(link, bytes([0x21]) + b"\xAA" * 7)

    assert (
        pyisotp.get_last_receive_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_UNEXP_PDU
    )
```


