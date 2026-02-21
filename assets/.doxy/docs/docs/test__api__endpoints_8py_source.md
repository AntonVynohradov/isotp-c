

# File test\_api\_endpoints.py

[**File List**](files.md) **>** [**integration**](dir_9ddfaab4002c4e8cb6c25cc17bdef75b.md) **>** [**test\_api\_endpoints.py**](test__api__endpoints_8py.md)

[Go to the documentation of this file](test__api__endpoints_8py.md)


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


"""! @file test_api_endpoints.py
@brief Integration smoke tests for public API endpoints.
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
    """
    return bytes((idx % 256 for idx in range(size)))


def _ff_frame(payload_size, payload):
    """! @brief Build a First Frame (FF) for an ISO-TP multi-frame transfer.
    @param payload_size Total payload size.
    @param payload Payload bytes to embed in FF.
    @return CAN frame bytes representing the FF.
    """
    ff_dl_high = (payload_size >> 8) & 0x0F
    ff_dl_low = payload_size & 0xFF
    return bytes([0x10 | ff_dl_high, ff_dl_low]) + payload[:6]


def _cf_frame(sn, payload):
    """! @brief Build a Consecutive Frame (CF) with a given sequence number.
    @param sn Sequence number (0-15).
    @param payload Payload bytes to embed in CF.
    @return CAN frame bytes representing the CF.
    """
    return bytes([0x20 | (sn & 0x0F)]) + payload[:7]


def test_api_endpoints_smoke():
    """! @brief Verify basic API calls and protocol result handling."""
    link = pyisotp.init(0x700, 1024, 1024)

    assert isinstance(pyisotp.ISOTP_PROTOCOL_RESULT_TIMEOUT_BS, int)

    pyisotp.time_reset()
    pyisotp.time_set(1)
    pyisotp.time_advance(1)

    payload = b"\x01\x02\x03"
    pyisotp.send(link, payload)
    pyisotp.poll(link)
    resp = pyisotp.receive(link, 1024)
    assert resp == payload

    pyisotp.mock_enable_drop(True)
    pyisotp.send(link, payload)
    pyisotp.poll(link)
    resp = pyisotp.receive(link, 1024)
    assert resp is None
    pyisotp.mock_enable_drop(False)

    pyisotp.mock_disable_fc(True)
    pyisotp.time_set(0)
    pyisotp.send(link, bytes(range(12)))
    for _ in range(101):
        pyisotp.time_advance(1)
        pyisotp.poll(link)

    assert (
        pyisotp.get_last_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_TIMEOUT_BS
    )
    pyisotp.mock_disable_fc(False)


def test_api_endpoints_smoke_multi_frame():
    """! @brief Verify multi-frame send/receive path."""
    link = pyisotp.init(0x700, 1024, 1024)
    payload = _make_payload(20)

    pyisotp.send(link, payload)
    resp = _poll_until_receive(link, 1024, 5)
    assert resp == payload


def test_api_endpoints_return_codes():
    """! @brief Validate return codes for overflow and in-progress sends."""
    link = pyisotp.init(0x700, 16, 16)
    payload = _make_payload(20)

    ret = pyisotp.send(link, payload)
    assert ret == pyisotp.ISOTP_RET_OVERFLOW

    payload = _make_payload(12)
    first = pyisotp.send(link, payload)
    second = pyisotp.send(link, payload)

    assert first == 0
    assert second == pyisotp.ISOTP_RET_INPROGRESS


def test_api_endpoints_validation_errors():
    """! @brief Validate input checks and error signaling in API."""
    with pytest.raises(ValueError):
        pyisotp.init(0x700, 0, 16)

    link = pyisotp.init(0x700, 64, 64)
    with pytest.raises(ValueError):
        pyisotp.receive(link, 0)

    with pytest.raises(ValueError):
        pyisotp.set_fc_params(link, 256, 0)


def test_api_endpoints_inject_unexpected_cf():
    """! @brief Validate unexpected CF injection handling."""
    link = pyisotp.init(0x700, 64, 64)
    pyisotp.inject_can(link, _cf_frame(1, b"\xAA" * 7))

    assert (
        pyisotp.get_last_receive_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_UNEXP_PDU
    )


def test_api_endpoints_inject_wrong_sn():
    """! @brief Validate wrong sequence number handling."""
    link = pyisotp.init(0x700, 64, 64)
    payload = _make_payload(20)
    pyisotp.inject_can(link, _ff_frame(len(payload), payload))
    pyisotp.inject_can(link, _cf_frame(5, payload[6:13]))

    assert (
        pyisotp.get_last_receive_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_WRONG_SN
    )
```


