/**
 * @file gtest_isotp_decode.cpp
 * @brief Unit tests: isotp_decode() for every frame type and every rejection.
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

namespace
{
int decode(std::vector<uint8_t> frame, struct isotp_pdu* pdu)
{
    return isotp_decode(frame.data(), frame.size(), pdu);
}
}  // namespace

TEST(IsotpDecode, RejectsNullAndBadLength)
{
    struct isotp_pdu pdu;
    const uint8_t frame[9] = {0x01U, 0xAAU};

    EXPECT_EQ(isotp_decode(nullptr, 2U, &pdu), ISOTP_ERROR);
    EXPECT_EQ(isotp_decode(frame, 2U, nullptr), ISOTP_ERROR);
    EXPECT_EQ(isotp_decode(frame, 0U, &pdu), ISOTP_ERROR);
    EXPECT_EQ(isotp_decode(frame, 9U, &pdu), ISOTP_ERROR);
}

TEST(IsotpDecode, SingleFrame)
{
    struct isotp_pdu pdu;

    /* UDS DiagnosticSessionControl, extended session: 02 10 03 */
    ASSERT_EQ(decode({0x02U, 0x10U, 0x03U}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.type, ISOTP_PCI_SINGLE_FRAME);
    EXPECT_EQ(pdu.message_length, 2U);
    EXPECT_EQ(pdu.data_offset, 1U);
    EXPECT_EQ(pdu.data_length, 2U);

    /* Padded frame: the CAN DL may exceed SF_DL + 1 */
    ASSERT_EQ(decode({0x07U, 1, 2, 3, 4, 5, 6, 7}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.message_length, 7U);
}

TEST(IsotpDecode, SingleFrameRejected)
{
    struct isotp_pdu pdu;

    EXPECT_EQ(decode({0x00U, 0xAAU}, &pdu), ISOTP_ERROR);        /* SF_DL = 0 */
    EXPECT_EQ(decode({0x03U, 0xAAU, 0xBBU}, &pdu), ISOTP_ERROR); /* SF_DL > CAN DL - 1 */
    EXPECT_EQ(decode({0x08U, 1, 2, 3, 4, 5, 6, 7}, &pdu), ISOTP_ERROR);
}

TEST(IsotpDecode, FirstFrameShort)
{
    struct isotp_pdu pdu;

    /* 20-byte message: 10 14 + 6 data bytes */
    ASSERT_EQ(decode({0x10U, 0x14U, 0, 1, 2, 3, 4, 5}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.type, ISOTP_PCI_FIRST_FRAME);
    EXPECT_EQ(pdu.message_length, 20U);
    EXPECT_EQ(pdu.data_offset, 2U);
    EXPECT_EQ(pdu.data_length, 6U);

    ASSERT_EQ(decode({0x1FU, 0xFFU, 0, 1, 2, 3, 4, 5}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.message_length, 4095U);
    ASSERT_EQ(decode({0x10U, 0x08U, 0, 1, 2, 3, 4, 5}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.message_length, 8U);
}

TEST(IsotpDecode, FirstFrameEscape)
{
    struct isotp_pdu pdu;

    /* 4096-byte message: 10 00 00 00 10 00 + 2 data bytes */
    ASSERT_EQ(decode({0x10U, 0x00U, 0x00U, 0x00U, 0x10U, 0x00U, 0xA0U, 0xA1U}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.message_length, 4096U);
    EXPECT_EQ(pdu.data_offset, 6U);
    EXPECT_EQ(pdu.data_length, 2U);
}

TEST(IsotpDecode, FirstFrameRejected)
{
    struct isotp_pdu pdu;

    EXPECT_EQ(decode({0x10U, 0x14U, 0, 1, 2, 3, 4}, &pdu), ISOTP_ERROR);    /* not 8 bytes */
    EXPECT_EQ(decode({0x10U, 0x07U, 0, 1, 2, 3, 4, 5}, &pdu), ISOTP_ERROR); /* fits an SF */
    EXPECT_EQ(decode({0x10U, 0x00U, 0x00U, 0x00U, 0x0FU, 0xFFU, 0, 1}, &pdu), ISOTP_ERROR);
}

TEST(IsotpDecode, ConsecutiveFrame)
{
    struct isotp_pdu pdu;

    ASSERT_EQ(decode({0x21U, 6, 7, 8}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.type, ISOTP_PCI_CONSECUTIVE_FRAME);
    EXPECT_EQ(pdu.sequence_number, 1U);
    EXPECT_EQ(pdu.data_offset, 1U);
    EXPECT_EQ(pdu.data_length, 3U);

    ASSERT_EQ(decode({0x2FU, 1, 2, 3, 4, 5, 6, 7}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.sequence_number, 15U);
    EXPECT_EQ(pdu.data_length, 7U);

    EXPECT_EQ(decode({0x21U}, &pdu), ISOTP_ERROR); /* no data */
}

TEST(IsotpDecode, FlowControl)
{
    struct isotp_pdu pdu;

    ASSERT_EQ(decode({0x30U, 0x08U, 0x14U}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.type, ISOTP_PCI_FLOW_CONTROL);
    EXPECT_EQ(pdu.flow_status, ISOTP_FS_CONTINUE);
    EXPECT_EQ(pdu.block_size, 8U);
    EXPECT_EQ(pdu.st_min, 0x14U);

    /* Unknown flow status is passed through for the sender to report */
    ASSERT_EQ(decode({0x35U, 0x00U, 0x00U, 0xCCU, 0xCCU}, &pdu), ISOTP_OK);
    EXPECT_EQ(pdu.flow_status, 5U);

    EXPECT_EQ(decode({0x30U, 0x00U}, &pdu), ISOTP_ERROR); /* shorter than 3 bytes */
}

TEST(IsotpDecode, ReservedTypesRejected)
{
    struct isotp_pdu pdu;

    for (unsigned type = 4U; type <= 15U; type++)
    {
        EXPECT_EQ(decode({static_cast<uint8_t>(type << 4U), 0, 0, 0, 0, 0, 0, 0}, &pdu),
                  ISOTP_ERROR)
            << "type " << type;
    }
}
