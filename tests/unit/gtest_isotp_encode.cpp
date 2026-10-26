/**
 * @file gtest_isotp_encode.cpp
 * @brief Unit tests: frame encoders and padding.
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
std::vector<uint8_t> bytes(const uint8_t* frame, int length)
{
    return std::vector<uint8_t>(frame, frame + length);
}
}  // namespace

TEST(IsotpEncode, SingleFrame)
{
    uint8_t frame[8];
    const uint8_t payload[] = {0x10U, 0x03U};

    ASSERT_EQ(isotp_encode_single(frame, sizeof(frame), payload, 2U), 3);
    EXPECT_EQ(bytes(frame, 3), (std::vector<uint8_t>{0x02U, 0x10U, 0x03U}));

    const std::vector<uint8_t> seven = pattern(7);
    ASSERT_EQ(isotp_encode_single(frame, 8U, seven.data(), 7U), 8);
    EXPECT_EQ(frame[0], 0x07U);
}

TEST(IsotpEncode, SingleFrameRejected)
{
    uint8_t frame[8];
    const std::vector<uint8_t> data = pattern(8);

    EXPECT_EQ(isotp_encode_single(nullptr, 8U, data.data(), 2U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_single(frame, 8U, nullptr, 2U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_single(frame, 8U, data.data(), 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_single(frame, 8U, data.data(), 8U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_single(frame, 2U, data.data(), 2U), ISOTP_ERROR); /* one byte short */
    EXPECT_EQ(isotp_encode_single(frame, 3U, data.data(), 2U), 3);           /* exact fit */
}

TEST(IsotpEncode, FirstFrameShort)
{
    uint8_t frame[8];
    const std::vector<uint8_t> data = pattern(20);

    ASSERT_EQ(isotp_encode_first(frame, sizeof(frame), data.data(), 20U), 8);
    EXPECT_EQ(bytes(frame, 8), (std::vector<uint8_t>{0x10U, 0x14U, 0, 1, 2, 3, 4, 5}));

    ASSERT_EQ(isotp_encode_first(frame, sizeof(frame), data.data(), 4095U), 8);
    EXPECT_EQ(frame[0], 0x1FU);
    EXPECT_EQ(frame[1], 0xFFU);
}

TEST(IsotpEncode, FirstFrameEscape)
{
    uint8_t frame[8];
    const std::vector<uint8_t> data = pattern(2);

    ASSERT_EQ(isotp_encode_first(frame, sizeof(frame), data.data(), 4096U), 8);
    EXPECT_EQ(bytes(frame, 8),
              (std::vector<uint8_t>{0x10U, 0x00U, 0x00U, 0x00U, 0x10U, 0x00U, 0x00U, 0x01U}));

    ASSERT_EQ(isotp_encode_first(frame, sizeof(frame), data.data(), 0xFFFFFFFFUL), 8);
    EXPECT_EQ(isotp_get_u32_be(&frame[2]), 0xFFFFFFFFUL);
}

TEST(IsotpEncode, FirstFrameRejected)
{
    uint8_t frame[8];
    const std::vector<uint8_t> data = pattern(8);

    EXPECT_EQ(isotp_encode_first(nullptr, 8U, data.data(), 20U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_first(frame, 8U, nullptr, 20U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_first(frame, 8U, data.data(), 7U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_first(frame, 7U, data.data(), 20U), ISOTP_ERROR);
}

TEST(IsotpEncode, ConsecutiveFrame)
{
    uint8_t frame[8];
    const std::vector<uint8_t> data = pattern(7);

    ASSERT_EQ(isotp_encode_consecutive(frame, sizeof(frame), 1U, data.data(), 7U), 8);
    EXPECT_EQ(bytes(frame, 8), (std::vector<uint8_t>{0x21U, 0, 1, 2, 3, 4, 5, 6}));

    ASSERT_EQ(isotp_encode_consecutive(frame, sizeof(frame), 15U, data.data(), 1U), 2);
    EXPECT_EQ(bytes(frame, 2), (std::vector<uint8_t>{0x2FU, 0x00U}));
}

TEST(IsotpEncode, ConsecutiveFrameRejected)
{
    uint8_t frame[8];
    const std::vector<uint8_t> data = pattern(8);

    EXPECT_EQ(isotp_encode_consecutive(nullptr, 8U, 1U, data.data(), 7U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_consecutive(frame, 8U, 1U, nullptr, 7U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_consecutive(frame, 8U, 16U, data.data(), 7U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_consecutive(frame, 8U, 1U, data.data(), 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_consecutive(frame, 8U, 1U, data.data(), 8U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_consecutive(frame, 7U, 1U, data.data(), 7U), ISOTP_ERROR);
}

TEST(IsotpEncode, FlowControl)
{
    uint8_t frame[8];

    ASSERT_EQ(isotp_encode_flow_control(frame, sizeof(frame), ISOTP_FS_CONTINUE, 8U, 0x14U), 3);
    EXPECT_EQ(bytes(frame, 3), (std::vector<uint8_t>{0x30U, 0x08U, 0x14U}));
    ASSERT_EQ(isotp_encode_flow_control(frame, 3U, ISOTP_FS_OVERFLOW, 0U, 0U), 3);
    EXPECT_EQ(frame[0], 0x32U);

    EXPECT_EQ(isotp_encode_flow_control(nullptr, 8U, ISOTP_FS_WAIT, 0U, 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_flow_control(frame, 8U, 3U, 0U, 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_encode_flow_control(frame, 2U, ISOTP_FS_WAIT, 0U, 0U), ISOTP_ERROR);
}

TEST(IsotpEncode, Padding)
{
    uint8_t frame[8] = {0x30U, 0x00U, 0x00U};

    ASSERT_EQ(isotp_frame_pad(frame, sizeof(frame), 3U, 0xCCU), 8);
    EXPECT_EQ(bytes(frame, 8),
              (std::vector<uint8_t>{0x30U, 0, 0, 0xCCU, 0xCCU, 0xCCU, 0xCCU, 0xCCU}));
    EXPECT_EQ(isotp_frame_pad(frame, sizeof(frame), 8U, 0x55U), 8); /* already full */
    EXPECT_EQ(frame[7], 0xCCU);

    EXPECT_EQ(isotp_frame_pad(nullptr, 8U, 3U, 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_frame_pad(frame, 7U, 3U, 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_frame_pad(frame, 8U, 0U, 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_frame_pad(frame, 8U, 9U, 0U), ISOTP_ERROR);
}

TEST(IsotpEncode, EncodeDecodeRoundTrip)
{
    uint8_t frame[8];
    struct isotp_pdu pdu;
    const std::vector<uint8_t> data = pattern(7);

    for (uint32_t length = 1U; length <= 7U; length++)
    {
        int n = isotp_encode_single(frame, sizeof(frame), data.data(), length);
        ASSERT_EQ(isotp_decode(frame, static_cast<size_t>(n), &pdu), ISOTP_OK);
        EXPECT_EQ(pdu.message_length, length);
    }
    for (uint32_t length : {8U, 100U, 4095U, 4096U, 70000U})
    {
        int n = isotp_encode_first(frame, sizeof(frame), data.data(), length);
        ASSERT_EQ(isotp_decode(frame, static_cast<size_t>(n), &pdu), ISOTP_OK);
        EXPECT_EQ(pdu.message_length, length);
    }
}
