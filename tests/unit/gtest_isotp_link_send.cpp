/**
 * @file gtest_isotp_link_send.cpp
 * @brief Unit tests: link transmitter (SF, FF, CF, flow control, timeouts).
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
/* Unit tests are built with ISOTP_TX_BUFFER_SIZE = ISOTP_RX_BUFFER_SIZE = 5000 (isotp.cmake). */
static_assert(ISOTP_TX_BUFFER_SIZE == 5000U, "unit tests expect 5000-byte buffers");

struct LinkSend : public LinkTest
{
    /* Send a message and grant it with a Flow Control frame. */
    void start(size_t length, std::vector<uint8_t> fc, uint32_t now_us = 0U)
    {
        data = pattern(length);
        ASSERT_EQ(isotp_link_send(link.get(), data.data(), static_cast<uint32_t>(length), now_us),
                  ISOTP_OK);
        feed(link.get(), fc, now_us);
    }

    /* Poll until the transmission ends, at most max_polls times; returns the polls used. */
    int drain(uint32_t now_us = 0U, int max_polls = 1000)
    {
        int polls = 0;
        while (isotp_link_tx_busy(link.get()) && (polls < max_polls))
        {
            isotp_link_poll(link.get(), now_us);
            polls++;
        }
        return polls;
    }

    /* Payload carried by the captured frames (after the FF). */
    std::vector<uint8_t> sent_payload() const
    {
        std::vector<uint8_t> out;
        for (const CapturedFrame& frame : capture.frames)
        {
            uint8_t type = static_cast<uint8_t>(frame.data[0] >> 4U);
            if (type == 1U)
            {
                size_t offset = (frame.data[1] == 0U && (frame.data[0] & 0x0FU) == 0U) ? 6U : 2U;
                out.insert(out.end(), frame.data.begin() + static_cast<long>(offset),
                           frame.data.end());
            }
            else if (type == 2U)
            {
                out.insert(out.end(), frame.data.begin() + 1, frame.data.end());
            }
        }
        out.resize(data.size());
        return out;
    }

    std::vector<uint8_t> data;
};
}  // namespace

TEST_F(LinkSend, SingleFrame)
{
    const uint8_t payload[] = {0x10U, 0x03U};

    ASSERT_EQ(isotp_link_send(link.get(), payload, 2U, 0U), ISOTP_OK);
    ASSERT_EQ(capture.frames.size(), 1U);
    EXPECT_EQ(capture.frames[0].can_id, 0x7E0U);
    EXPECT_EQ(capture.frames[0].data, (std::vector<uint8_t>{0x02U, 0x10U, 0x03U}));
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_OK}));
    EXPECT_FALSE(isotp_link_tx_busy(link.get()));
}

TEST_F(LinkSend, SingleFramePadded)
{
    const uint8_t payload[] = {0x3EU, 0x00U};

    config.padding = true;
    config.padding_value = 0xAAU;
    reinit();
    ASSERT_EQ(isotp_link_send(link.get(), payload, 2U, 0U), ISOTP_OK);
    EXPECT_EQ(capture.frames[0].data,
              (std::vector<uint8_t>{0x02U, 0x3EU, 0x00U, 0xAAU, 0xAAU, 0xAAU, 0xAAU, 0xAAU}));
}

TEST_F(LinkSend, SingleFrameWriterError)
{
    const uint8_t payload[] = {0x01U};

    capture.write_results = {-42};
    EXPECT_EQ(isotp_link_send(link.get(), payload, 1U, 0U), -42);
    capture.write_results = {ISOTP_BUSY};
    EXPECT_EQ(isotp_link_send(link.get(), payload, 1U, 0U), ISOTP_BUSY);
    EXPECT_TRUE(capture.tx_done.empty());
}

TEST_F(LinkSend, SingleFrameWithoutCallback)
{
    const uint8_t payload[] = {0x01U};

    config.on_tx_done = nullptr;
    reinit();
    EXPECT_EQ(isotp_link_send(link.get(), payload, 1U, 0U), ISOTP_OK);
    EXPECT_EQ(isotp_link_tx_result(link.get()), ISOTP_N_OK);
}

TEST_F(LinkSend, RejectsInvalidArguments)
{
    const std::vector<uint8_t> big = pattern(ISOTP_TX_BUFFER_SIZE + 1U);

    EXPECT_EQ(isotp_link_send(nullptr, big.data(), 1U, 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_link_send(link.get(), nullptr, 1U, 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_link_send(link.get(), big.data(), 0U, 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_link_send(link.get(), big.data(), ISOTP_TX_BUFFER_SIZE + 1U, 0U), ISOTP_ERROR);
    EXPECT_TRUE(capture.frames.empty());
}

TEST_F(LinkSend, FirstFrameWaitsForFlowControl)
{
    data = pattern(20);
    ASSERT_EQ(isotp_link_send(link.get(), data.data(), 20U, 0U), ISOTP_OK);
    ASSERT_EQ(capture.frames.size(), 1U);
    EXPECT_EQ(capture.frames[0].data, (std::vector<uint8_t>{0x10U, 0x14U, 0, 1, 2, 3, 4, 5}));
    EXPECT_TRUE(isotp_link_tx_busy(link.get()));
    EXPECT_EQ(isotp_link_send(link.get(), data.data(), 20U, 0U), ISOTP_BUSY);

    isotp_link_poll(link.get(), 1000U); /* no CF before Flow Control */
    EXPECT_EQ(capture.frames.size(), 1U);
}

TEST_F(LinkSend, FirstFrameWriterError)
{
    data = pattern(20);
    capture.write_results = {-7};
    EXPECT_EQ(isotp_link_send(link.get(), data.data(), 20U, 0U), -7);
    EXPECT_FALSE(isotp_link_tx_busy(link.get()));
}

TEST_F(LinkSend, MultiFrameUnlimitedBlock)
{
    start(20U, {0x30U, 0x00U, 0x00U});
    EXPECT_EQ(drain(), 2);
    ASSERT_EQ(capture.frames.size(), 3U);
    EXPECT_EQ(capture.frames[1].data, (std::vector<uint8_t>{0x21U, 6, 7, 8, 9, 10, 11, 12}));
    EXPECT_EQ(capture.frames[2].data, (std::vector<uint8_t>{0x22U, 13, 14, 15, 16, 17, 18, 19}));
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_OK}));
    EXPECT_EQ(isotp_link_tx_result(link.get()), ISOTP_N_OK);
}

TEST_F(LinkSend, EightByteMessage)
{
    start(8U, {0x30U, 0x00U, 0x00U});
    drain();
    ASSERT_EQ(capture.frames.size(), 2U);
    EXPECT_EQ(capture.frames[1].data, (std::vector<uint8_t>{0x21U, 6, 7}));
}

TEST_F(LinkSend, SequenceNumberWraps)
{
    start(6U + 7U * 17U, {0x30U, 0x00U, 0x00U});
    drain();
    ASSERT_EQ(capture.frames.size(), 18U);
    EXPECT_EQ(capture.frames[15].data[0], 0x2FU);
    EXPECT_EQ(capture.frames[16].data[0], 0x20U);
    EXPECT_EQ(capture.frames[17].data[0], 0x21U);
    EXPECT_EQ(sent_payload(), data);
}

TEST_F(LinkSend, BlockSizeRequiresNextFlowControl)
{
    start(40U, {0x30U, 0x03U, 0x00U}, 0U); /* 40 = 6 + 7 * 4 + 6: FF + 5 CF */
    drain(10U, 5);
    EXPECT_EQ(capture.frames.size(), 4U); /* FF + 3 CF, then waiting */
    EXPECT_TRUE(isotp_link_tx_busy(link.get()));

    feed(link.get(), {0x30U, 0x03U, 0x00U}, 20U);
    drain(20U);
    EXPECT_EQ(capture.frames.size(), 6U);
    EXPECT_EQ(sent_payload(), data);
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_OK}));
}

TEST_F(LinkSend, TimeoutBetweenBlocks)
{
    config.n_bs_timeout_us = 1000U;
    reinit();
    start(40U, {0x30U, 0x01U, 0x00U}, 0U);
    isotp_link_poll(link.get(), 100U); /* CF 1, block done at t = 100 */
    isotp_link_poll(link.get(), 1099U);
    EXPECT_TRUE(isotp_link_tx_busy(link.get()));
    isotp_link_poll(link.get(), 1100U);
    EXPECT_FALSE(isotp_link_tx_busy(link.get()));
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_TIMEOUT_BS}));
}

TEST_F(LinkSend, TimeoutAfterFirstFrame)
{
    config.n_bs_timeout_us = 1000U;
    reinit();
    data = pattern(20);
    ASSERT_EQ(isotp_link_send(link.get(), data.data(), 20U, 5000U), ISOTP_OK);
    isotp_link_poll(link.get(), 5999U);
    EXPECT_TRUE(isotp_link_tx_busy(link.get()));
    isotp_link_poll(link.get(), 6000U);
    EXPECT_EQ(isotp_link_tx_result(link.get()), ISOTP_N_TIMEOUT_BS);
}

TEST_F(LinkSend, TimeoutAcrossClockWrap)
{
    config.n_bs_timeout_us = 1000U;
    reinit();
    data = pattern(20);
    ASSERT_EQ(isotp_link_send(link.get(), data.data(), 20U, 0xFFFFFF00UL), ISOTP_OK);
    isotp_link_poll(link.get(), 0xFFFFFFF0UL);
    isotp_link_poll(link.get(), 0x000002E7UL); /* 999 us after the send */
    EXPECT_TRUE(isotp_link_tx_busy(link.get()));
    isotp_link_poll(link.get(), 0x000002E8UL);
    EXPECT_EQ(isotp_link_tx_result(link.get()), ISOTP_N_TIMEOUT_BS);
}

TEST_F(LinkSend, SeparationTime)
{
    start(30U, {0x30U, 0x00U, 0x14U}, 0U); /* STmin 20 ms */
    isotp_link_poll(link.get(), 0U);
    EXPECT_EQ(capture.frames.size(), 2U);
    isotp_link_poll(link.get(), 19999U);
    EXPECT_EQ(capture.frames.size(), 2U);
    isotp_link_poll(link.get(), 20000U);
    EXPECT_EQ(capture.frames.size(), 3U);
}

TEST_F(LinkSend, SeparationTimeMicroseconds)
{
    start(30U, {0x30U, 0x00U, 0xF5U}, 0U); /* STmin 500 us */
    isotp_link_poll(link.get(), 0U);
    isotp_link_poll(link.get(), 499U);
    EXPECT_EQ(capture.frames.size(), 2U);
    isotp_link_poll(link.get(), 500U);
    EXPECT_EQ(capture.frames.size(), 3U);
}

TEST_F(LinkSend, WaitFramesWithinLimit)
{
    config.wft_max = 2U;
    config.n_bs_timeout_us = 1000U;
    reinit();
    start(20U, {0x31U, 0x00U, 0x00U}, 0U);
    feed(link.get(), {0x31U, 0x00U, 0x00U}, 900U);
    isotp_link_poll(link.get(), 1800U); /* N_Bs restarted at t = 900 */
    EXPECT_TRUE(isotp_link_tx_busy(link.get()));
    feed(link.get(), {0x30U, 0x00U, 0x00U}, 1800U);
    drain(1800U);
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_OK}));
}

TEST_F(LinkSend, WaitFrameOverrun)
{
    config.wft_max = 1U;
    reinit();
    start(20U, {0x31U, 0x00U, 0x00U});
    EXPECT_TRUE(isotp_link_tx_busy(link.get()));
    feed(link.get(), {0x31U, 0x00U, 0x00U});
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_WFT_OVRN}));
}

TEST_F(LinkSend, ReceiverOverflow)
{
    start(20U, {0x32U, 0x00U, 0x00U});
    EXPECT_FALSE(isotp_link_tx_busy(link.get()));
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_BUFFER_OVFLW}));
}

TEST_F(LinkSend, InvalidFlowStatus)
{
    start(20U, {0x37U, 0x00U, 0x00U});
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_INVALID_FS}));
}

TEST_F(LinkSend, UnexpectedFlowControlIgnored)
{
    feed(link.get(), {0x30U, 0x00U, 0x00U}); /* idle */
    EXPECT_FALSE(isotp_link_tx_busy(link.get()));

    start(30U, {0x30U, 0x00U, 0x14U}, 0U);
    isotp_link_poll(link.get(), 0U);
    feed(link.get(), {0x32U, 0x00U, 0x00U}, 1U); /* sending CFs: ignored */
    EXPECT_TRUE(isotp_link_tx_busy(link.get()));
    EXPECT_EQ(isotp_link_invalid_frames(link.get()), 0U);
}

TEST_F(LinkSend, ConsecutiveFrameWriterBusyRetries)
{
    start(20U, {0x30U, 0x00U, 0x00U});
    capture.write_results = {ISOTP_BUSY};
    isotp_link_poll(link.get(), 0U);
    EXPECT_EQ(capture.frames.size(), 1U);
    drain();
    ASSERT_EQ(capture.frames.size(), 3U);
    EXPECT_EQ(capture.frames[1].data[0], 0x21U);
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_OK}));
}

TEST_F(LinkSend, ConsecutiveFrameWriterError)
{
    start(20U, {0x30U, 0x00U, 0x00U});
    capture.write_results = {-5};
    isotp_link_poll(link.get(), 0U);
    EXPECT_FALSE(isotp_link_tx_busy(link.get()));
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_ERROR}));
}

TEST_F(LinkSend, ErrorWithoutCallback)
{
    config.on_tx_done = nullptr;
    reinit();
    start(20U, {0x32U, 0x00U, 0x00U});
    EXPECT_EQ(isotp_link_tx_result(link.get()), ISOTP_N_BUFFER_OVFLW);
}

TEST_F(LinkSend, LongMessageUsesEscapeSequence)
{
    start(4500U, {0x30U, 0x00U, 0x00U});
    EXPECT_EQ(capture.frames[0].data,
              (std::vector<uint8_t>{0x10U, 0x00U, 0x00U, 0x00U, 0x11U, 0x94U, 0x00U, 0x01U}));
    drain();
    EXPECT_EQ(capture.frames.size(), 1U + 643U); /* 4498 = 7 * 642 + 4 */
    EXPECT_EQ(sent_payload(), data);
}

TEST_F(LinkSend, PaddedConsecutiveFrames)
{
    config.padding = true;
    reinit();
    start(9U, {0x30U, 0x00U, 0x00U});
    drain();
    EXPECT_EQ(capture.frames[1].data,
              (std::vector<uint8_t>{0x21U, 6, 7, 8, 0xCCU, 0xCCU, 0xCCU, 0xCCU}));
}

TEST_F(LinkSend, Functional)
{
    const uint8_t payload[] = {0x3EU, 0x80U};
    const std::vector<uint8_t> eight = pattern(8);

    ASSERT_EQ(isotp_link_send_functional(link.get(), 0x7DFU, payload, 2U), ISOTP_OK);
    EXPECT_EQ(capture.frames[0].can_id, 0x7DFU);
    EXPECT_EQ(capture.frames[0].data, (std::vector<uint8_t>{0x02U, 0x3EU, 0x80U}));
    EXPECT_TRUE(capture.tx_done.empty());

    EXPECT_EQ(isotp_link_send_functional(nullptr, 0x7DFU, payload, 2U), ISOTP_ERROR);
    EXPECT_EQ(isotp_link_send_functional(link.get(), 0x7DFU, nullptr, 2U), ISOTP_ERROR);
    EXPECT_EQ(isotp_link_send_functional(link.get(), 0x7DFU, payload, 0U), ISOTP_ERROR);
    EXPECT_EQ(isotp_link_send_functional(link.get(), 0x7DFU, eight.data(), 8U), ISOTP_ERROR);
    capture.write_results = {-3};
    EXPECT_EQ(isotp_link_send_functional(link.get(), 0x7DFU, payload, 2U), -3);
}

TEST_F(LinkSend, FunctionalDuringSegmentedTransmission)
{
    const uint8_t payload[] = {0x3EU, 0x80U};

    start(20U, {0x30U, 0x00U, 0x00U});
    ASSERT_EQ(isotp_link_send_functional(link.get(), 0x7DFU, payload, 2U), ISOTP_OK);
    drain();
    EXPECT_EQ(sent_payload(), data);
}
