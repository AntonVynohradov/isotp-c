/**
 * @file gtest_isotp_st_min.cpp
 * @brief Unit tests: STmin encoding and decoding.
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

#include "isotp_test_support.h"

TEST(IsotpStMin, Encode)
{
    EXPECT_EQ(isotp_st_min_encode(0U), 0x00U);
    EXPECT_EQ(isotp_st_min_encode(1U), 0xF1U); /* rounded up to 100 us */
    EXPECT_EQ(isotp_st_min_encode(100U), 0xF1U);
    EXPECT_EQ(isotp_st_min_encode(101U), 0xF2U);
    EXPECT_EQ(isotp_st_min_encode(900U), 0xF9U);
    EXPECT_EQ(isotp_st_min_encode(901U), 0x01U); /* rounded up to 1 ms */
    EXPECT_EQ(isotp_st_min_encode(1000U), 0x01U);
    EXPECT_EQ(isotp_st_min_encode(20000U), 0x14U);
    EXPECT_EQ(isotp_st_min_encode(126001U), 0x7FU);
    EXPECT_EQ(isotp_st_min_encode(127000U), 0x7FU);
    EXPECT_EQ(isotp_st_min_encode(0xFFFFFFFFUL), 0x7FU);
}

TEST(IsotpStMin, Decode)
{
    EXPECT_EQ(isotp_st_min_decode(0x00U), 0U);
    EXPECT_EQ(isotp_st_min_decode(0x14U), 20000U);
    EXPECT_EQ(isotp_st_min_decode(0x7FU), 127000U);
    EXPECT_EQ(isotp_st_min_decode(0xF1U), 100U);
    EXPECT_EQ(isotp_st_min_decode(0xF9U), 900U);
}

TEST(IsotpStMin, ReservedValuesDecodeAs127ms)
{
    for (unsigned value = 0x80U; value <= 0xF0U; value++)
    {
        EXPECT_EQ(isotp_st_min_decode(static_cast<uint8_t>(value)), 127000U) << value;
    }
    for (unsigned value = 0xFAU; value <= 0xFFU; value++)
    {
        EXPECT_EQ(isotp_st_min_decode(static_cast<uint8_t>(value)), 127000U) << value;
    }
}

TEST(IsotpStMin, EncodeNeverRequestsLessThanAsked)
{
    for (uint32_t us = 0U; us <= 130000U; us += 7U)
    {
        uint32_t expected = (us > 127000U) ? 127000U : us;
        EXPECT_GE(isotp_st_min_decode(isotp_st_min_encode(us)), expected) << us;
    }
}
