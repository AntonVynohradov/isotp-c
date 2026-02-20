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


"""! @file test_multi_frame.py
@brief Integration tests for multi-frame ISO-TP transfers.
@details Covers boundary sizes, timeouts, and sequence handling.
@note Time advances rely on mock time helpers in the binding.
"""

# =============================================================================
# IMPORTS
# =============================================================================

import pyisotp
import pytest

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


def _ff_frame(payload_size, payload):
    """! @brief Build a First Frame (FF) for an ISO-TP transfer.
    @param payload_size Total payload size.
    @param payload Payload bytes to embed in FF.
    @return CAN frame bytes representing the FF.
    @details Encodes the 0x10 PCI with 12-bit length.
    """
    ff_dl_high = (payload_size >> 8) & 0x0F
    ff_dl_low = payload_size & 0xFF
    return bytes([0x10 | ff_dl_high, ff_dl_low]) + payload[:6]


def _cf_frame(sn, payload):
    """! @brief Build a Consecutive Frame (CF) with a given sequence number.
    @param sn Sequence number (0-15).
    @param payload Payload bytes to embed in CF.
    @return CAN frame bytes representing the CF.
    @details Encodes the 0x20 PCI with SN.
    """
    return bytes([0x20 | (sn & 0x0F)]) + payload[:7]


def test_multi_frame_roundtrip():
    """! @brief Verify multi-frame roundtrip delivery.
    @details Ensures payload integrity across segmented transfer.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    payload = b"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C"
    pyisotp.send(link, payload)
    resp = _poll_until_receive(link, 1024, 3)
    assert resp == payload


@pytest.mark.parametrize("size", [8, 9, 14, 15])
def test_multi_frame_boundary_sizes(size):
    """! @brief Verify multi-frame behavior around boundary sizes.
    @param size Payload size under test.
    @details Exercises sizes near the SF/FF boundary.
    """
    link = pyisotp.init(0x700, 2048, 2048)
    payload = _make_payload(size)
    pyisotp.send(link, payload)
    resp = _poll_until_receive(link, 2048, 5)
    assert resp == payload


def test_multi_frame_max_payload():
    """! @brief Verify maximum payload size roundtrip.
    @details Uses a large payload to stress segmentation and reassembly.
    """
    link = pyisotp.init(0x700, 4096, 4096)
    payload = _make_payload(4095)
    pyisotp.send(link, payload)
    resp = _poll_until_receive(link, 4096, 800)
    assert resp == payload


def test_multi_frame_bs_one_stmin():
    """! @brief Verify block size 1 with STmin pacing.
    @details Advances time between frames to satisfy STmin.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    pyisotp.set_fc_params(link, 1, 5)
    payload = _make_payload(30)
    pyisotp.send(link, payload)
    resp = _poll_until_receive(link, 1024, 20, advance_ms=1)
    assert resp == payload


def test_multi_frame_bs_unlimited_stmin():
    """! @brief Verify unlimited block size with STmin pacing.
    @details Exercises BS=0 with STmin delay.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    pyisotp.set_fc_params(link, 0, 5)
    payload = _make_payload(30)
    pyisotp.send(link, payload)
    resp = _poll_until_receive(link, 1024, 20, advance_ms=1)
    assert resp == payload


def test_multi_frame_timeout_n_bs():
    """! @brief Verify N_Bs timeout during multi-frame send.
    @details Disables FC to force sender block-size timeout.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    pyisotp.set_timeouts(link, 50, 50)
    pyisotp.mock_disable_fc(True)

    pyisotp.send(link, _make_payload(30))
    for _ in range(51):
        pyisotp.time_advance(1)
        pyisotp.poll(link)

    assert pyisotp.get_last_protocol_result(link) == pyisotp.ISOTP_PROTOCOL_RESULT_TIMEOUT_BS


def test_multi_frame_timeout_n_cr():
    """! @brief Verify N_Cr timeout during multi-frame receive.
    @details Drops incoming frames to force receiver timeout.
    """
    link = pyisotp.init(0x700, 1024, 1024)
    pyisotp.set_timeouts(link, 50, 10)
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


def test_multi_frame_unexpected_cf():
    """! @brief Verify unexpected CF is reported.
    @details Injects a CF when receiver is idle.
    """
    link = pyisotp.init(0x700, 512, 512)
    pyisotp.inject_can(link, _cf_frame(1, b"\xAA" * 7))

    assert (
        pyisotp.get_last_receive_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_UNEXP_PDU
    )


def test_multi_frame_wrong_sn():
    """! @brief Verify wrong sequence number is reported.
    @details Injects a CF with unexpected SN after FF.
    """
    link = pyisotp.init(0x700, 512, 512)
    payload = _make_payload(20)
    pyisotp.inject_can(link, _ff_frame(len(payload), payload))
    pyisotp.inject_can(link, _cf_frame(5, payload[6:13]))

    assert (
        pyisotp.get_last_receive_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_WRONG_SN
    )
