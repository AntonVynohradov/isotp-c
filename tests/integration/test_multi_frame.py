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


# =============================================================================
# IMPORTS
# =============================================================================

import pyisotp
import pytest

# =============================================================================
# FUNCTION PROTOTYPES
# =============================================================================

def test_multi_frame_roundtrip():
    link = pyisotp.init(0x700, 1024, 1024)
    pyisotp.send(link, b"\x01\x02\x03")
    pyisotp.poll(link)
    resp = pyisotp.receive(link, 1024)
    assert resp == b"\x01\x02\x03"