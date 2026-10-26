/**
 * @file gtest_isotp_defs.cpp
 * @brief Unit tests: definitions, big-endian helpers and version macros.
 *
 * Project: ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
 * Author: Anton Vynohradov
 * Email: avynohradov@systemfromscratch.com
 *
 * @copyright Copyright (c) 2026 Anton Vynohradov
 * @license MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdio>

#include "isotp_test_support.h"

TEST(IsotpDefs, ProtocolValuesMatchIso15765)
{
    EXPECT_EQ(ISOTP_PCI_SINGLE_FRAME, 0x0U);
    EXPECT_EQ(ISOTP_PCI_FIRST_FRAME, 0x1U);
    EXPECT_EQ(ISOTP_PCI_CONSECUTIVE_FRAME, 0x2U);
    EXPECT_EQ(ISOTP_PCI_FLOW_CONTROL, 0x3U);
    EXPECT_EQ(ISOTP_FS_CONTINUE, 0x0U);
    EXPECT_EQ(ISOTP_FS_WAIT, 0x1U);
    EXPECT_EQ(ISOTP_FS_OVERFLOW, 0x2U);
    EXPECT_EQ(ISOTP_CAN_DL, 8U);
    EXPECT_EQ(ISOTP_SF_MAX_DL, 7U);
    EXPECT_EQ(ISOTP_FF_DL_12BIT_MAX, 4095U);
}

TEST(IsotpDefs, BigEndianRoundTrip)
{
    uint8_t bytes[4] = {0};

    isotp_put_u32_be(bytes, 0x12345678UL);
    EXPECT_EQ(bytes[0], 0x12U);
    EXPECT_EQ(bytes[1], 0x34U);
    EXPECT_EQ(bytes[2], 0x56U);
    EXPECT_EQ(bytes[3], 0x78U);
    EXPECT_EQ(isotp_get_u32_be(bytes), 0x12345678UL);

    isotp_put_u32_be(bytes, 0xFFFFFFFFUL);
    EXPECT_EQ(isotp_get_u32_be(bytes), 0xFFFFFFFFUL);
}

TEST(IsotpDefs, VersionStringMatchesNumbers)
{
    char expected[32];

    (void) std::snprintf(expected, sizeof(expected), "%d.%d.%d", ISOTP_VERSION_MAJOR,
                         ISOTP_VERSION_MINOR, ISOTP_VERSION_PATCH);
    EXPECT_STREQ(ISOTP_VERSION, expected);
}
