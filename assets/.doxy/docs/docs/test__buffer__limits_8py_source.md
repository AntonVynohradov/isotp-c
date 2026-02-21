

# File test\_buffer\_limits.py

[**File List**](files.md) **>** [**integration**](dir_9ddfaab4002c4e8cb6c25cc17bdef75b.md) **>** [**test\_buffer\_limits.py**](test__buffer__limits_8py.md)

[Go to the documentation of this file](test__buffer__limits_8py.md)


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


"""! @file test_buffer_limits.py
@brief Integration tests for buffer limit behavior.
@details Covers overflow, truncation, and in-progress paths for buffers.
@note Uses the Python wrapper to exercise C core behavior.
"""

# =============================================================================
# IMPORTS
# =============================================================================

import pyisotp

# =============================================================================
# FUNCTION PROTOTYPES
# =============================================================================


def _poll_until_receive(link, payload_size, steps):
    """! @brief Poll the link until a response is received or steps are exhausted.
    @param link ISO-TP link handle.
    @param payload_size Expected payload size for receive.
    @param steps Maximum number of poll iterations.
    @return Received payload bytes or None when nothing is received.
    @details Polls the core without advancing time.
    """
    resp = None
    for _ in range(steps):
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


# =============================================================================
# BUFFER LIMIT TESTS
# =============================================================================


def test_send_overflow_returns_code():
    """! @brief Verify overflow is reported on send.
    @details Exceeding the send buffer returns ISOTP_RET_OVERFLOW.
    """
    link = pyisotp.init(0x700, 8, 8)
    payload = _make_payload(9)

    ret = pyisotp.send(link, payload)
    assert ret == pyisotp.ISOTP_RET_OVERFLOW


def test_receive_truncates_payload():
    """! @brief Verify receive truncates payload to buffer size.
    @details The returned payload must be capped to the buffer length.
    """
    link = pyisotp.init(0x700, 128, 128)
    payload = _make_payload(20)

    pyisotp.send(link, payload)
    resp = _poll_until_receive(link, 10, 5)

    assert resp == payload[:10]


def test_send_in_progress_returns_code():
    """! @brief Verify in-progress is reported on concurrent send.
    @details Second send while active returns ISOTP_RET_INPROGRESS.
    """
    link = pyisotp.init(0x700, 128, 128)
    payload = _make_payload(20)

    first = pyisotp.send(link, payload)
    second = pyisotp.send(link, payload)

    assert first == 0
    assert second == pyisotp.ISOTP_RET_INPROGRESS
```


