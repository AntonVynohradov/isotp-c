

# File test\_timing.py

[**File List**](files.md) **>** [**integration**](dir_9ddfaab4002c4e8cb6c25cc17bdef75b.md) **>** [**test\_timing.py**](test__timing_8py.md)

[Go to the documentation of this file](test__timing_8py.md)


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


"""! @file test_timing.py
@brief Integration tests for timing-related ISO-TP behavior.
@details Covers N_Bs/N_Cr timeouts, STmin pacing, and BS=1 behavior.
@note Several placeholder tests are marked xfail until core support lands.
"""

# =============================================================================
# IMPORTS
# =============================================================================

import pyisotp
import pytest

# =============================================================================
# FUNCTION PROTOTYPES
# =============================================================================

def test_timeout_n_bs():
    """! @brief Verify N_Bs timeout handling.
    @details Disables FC to force sender timeout.
    """
    link = pyisotp.init(0x700, 512, 512)
    pyisotp.set_timeouts(link, 100, 100)
    pyisotp.mock_disable_fc(True)

    pyisotp.send(link, bytes(range(50)))
    for _ in range(101):
        pyisotp.time_advance(1)
        pyisotp.poll(link)

    # expect timeout according to ISO-15765
    assert pyisotp.get_last_protocol_result(link) == pyisotp.ISOTP_PROTOCOL_RESULT_TIMEOUT_BS


def test_timeout_n_cr():
    """! @brief Verify N_Cr timeout handling.
    @details Drops incoming frames to force receiver timeout.
    """
    link = pyisotp.init(0x700, 512, 512)
    pyisotp.set_timeouts(link, 100, 20)

    pyisotp.send(link, bytes(range(12)))
    pyisotp.poll(link)

    pyisotp.mock_enable_drop(True)
    for _ in range(21):
        pyisotp.time_advance(1)
        pyisotp.poll(link)

    assert (
        pyisotp.get_last_receive_protocol_result(link)
        == pyisotp.ISOTP_PROTOCOL_RESULT_TIMEOUT_CR
    )
    pyisotp.mock_enable_drop(False)


def test_st_min_enforced():
    """! @brief Verify STmin delay is enforced.
    @details Ensures receive does not complete before STmin elapses.
    """
    link = pyisotp.init(0x700, 512, 512)
    pyisotp.set_fc_params(link, 0, 10)

    payload = bytes(range(20))
    pyisotp.send(link, payload)
    pyisotp.poll(link)

    for _ in range(9):
        pyisotp.time_advance(1)
        pyisotp.poll(link)
        assert pyisotp.receive(link, 512) is None

    resp = None
    for _ in range(10):
        pyisotp.time_advance(1)
        pyisotp.poll(link)
        resp = pyisotp.receive(link, 512)
        if resp is not None:
            break

    assert resp == payload


def test_block_size_one_roundtrip():
    """! @brief Verify block size one roundtrip behavior.
    @details Uses BS=1 to enforce FC after each CF.
    """
    link = pyisotp.init(0x700, 512, 512)
    pyisotp.set_fc_params(link, 1, 0)

    payload = bytes(range(30))
    pyisotp.send(link, payload)

    resp = None
    for _ in range(10):
        pyisotp.poll(link)
        resp = pyisotp.receive(link, 512)
        if resp is not None:
            break

    assert resp == payload


@pytest.mark.xfail(reason="N_As not enforced by core yet")
def test_timeout_n_as_placeholder():
    """! @brief Placeholder for N_As timeout support.
    @note Marked xfail until core enforces N_As.
    """
    assert False


@pytest.mark.xfail(reason="N_Ar not enforced by core yet")
def test_timeout_n_ar_placeholder():
    """! @brief Placeholder for N_Ar timeout support.
    @note Marked xfail until core enforces N_Ar.
    """
    assert False


@pytest.mark.xfail(reason="N_Br not enforced by core yet")
def test_timeout_n_br_placeholder():
    """! @brief Placeholder for N_Br timeout support.
    @note Marked xfail until core enforces N_Br.
    """
    assert False


@pytest.mark.xfail(reason="N_Cs not enforced by core yet")
def test_timeout_n_cs_placeholder():
    """! @brief Placeholder for N_Cs timeout support.
    @note Marked xfail until core enforces N_Cs.
    """
    assert False
```


