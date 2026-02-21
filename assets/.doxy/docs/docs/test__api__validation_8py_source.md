

# File test\_api\_validation.py

[**File List**](files.md) **>** [**integration**](dir_9ddfaab4002c4e8cb6c25cc17bdef75b.md) **>** [**test\_api\_validation.py**](test__api__validation_8py.md)

[Go to the documentation of this file](test__api__validation_8py.md)


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


"""! @file test_api_validation.py
@brief Integration tests for API input validation.
@details Exercises guardrails for invalid inputs across init, receive, and FC params.
@note These tests expect Python bindings to raise ValueError.
"""

# =============================================================================
# IMPORTS
# =============================================================================

import pyisotp
import pytest

# =============================================================================
# FUNCTION PROTOTYPES
# =============================================================================


def test_init_rejects_zero_buffers():
    """! @brief Verify init rejects zero-length buffers.
    @details Validates both send and receive buffer size checks.
    """
    with pytest.raises(ValueError):
        pyisotp.init(0x700, 0, 16)

    with pytest.raises(ValueError):
        pyisotp.init(0x700, 16, 0)


def test_receive_rejects_invalid_bufsize():
    """! @brief Verify receive rejects invalid buffer sizes.
    @details Ensures zero length is rejected by the binding.
    """
    link = pyisotp.init(0x700, 64, 64)
    with pytest.raises(ValueError):
        pyisotp.receive(link, 0)


def test_set_fc_params_rejects_large_block_size():
    """! @brief Verify flow control params reject oversized block size.
    @details Block size values above 255 must be rejected.
    """
    link = pyisotp.init(0x700, 64, 64)
    with pytest.raises(ValueError):
        pyisotp.set_fc_params(link, 256, 0)
```


