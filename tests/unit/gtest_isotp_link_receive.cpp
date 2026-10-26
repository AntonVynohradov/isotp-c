/**
 * @file gtest_isotp_link_receive.cpp
 * @brief Unit tests: link receiver (SF, FF, CF, flow control, errors, timeouts).
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
struct LinkReceive : public LinkTest
{
    /* Segment a message the way a sender would: FF + CFs (unpadded). */
    static std::vector<std::vector<uint8_t>> segment(const std::vector<uint8_t>& message)
    {
        std::vector<std::vector<uint8_t>> frames;
        size_t offset;
        uint8_t sn = 1U;

        if (message.size() <= 4095U)
        {
            frames.push_back({static_cast<uint8_t>(0x10U | (message.size() >> 8U)),
                              static_cast<uint8_t>(message.size() & 0xFFU)});
            offset = 6U;
        }
        else
        {
            uint32_t n = static_cast<uint32_t>(message.size());
            frames.push_back({0x10U, 0x00U, static_cast<uint8_t>(n >> 24U),
                              static_cast<uint8_t>(n >> 16U), static_cast<uint8_t>(n >> 8U),
                              static_cast<uint8_t>(n)});
            offset = 2U;
        }
        frames[0].insert(frames[0].end(), message.begin(),
                         message.begin() + static_cast<long>(offset));
        while (offset < message.size())
        {
            size_t take = std::min<size_t>(7U, message.size() - offset);
            std::vector<uint8_t> cf{static_cast<uint8_t>(0x20U | sn)};
            cf.insert(cf.end(), message.begin() + static_cast<long>(offset),
                      message.begin() + static_cast<long>(offset + take));
            frames.push_back(cf);
            offset += take;
            sn = static_cast<uint8_t>((sn + 1U) & 0x0FU);
        }
        return frames;
    }
};
}  // namespace

TEST_F(LinkReceive, SingleFrame)
{
    feed(link.get(), {0x02U, 0x10U, 0x03U});
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], (std::vector<uint8_t>{0x10U, 0x03U}));
    EXPECT_EQ(isotp_link_rx_result(link.get()), ISOTP_N_OK);
    EXPECT_TRUE(capture.frames.empty());
}

TEST_F(LinkReceive, SingleFrameWithoutHandler)
{
    config.on_message = nullptr;
    reinit();
    feed(link.get(), {0x01U, 0x3EU});
    EXPECT_EQ(isotp_link_rx_result(link.get()), ISOTP_N_OK);
}

TEST_F(LinkReceive, MalformedFramesCounted)
{
    feed(link.get(), {0x00U, 0x01U});
    feed(link.get(), {0x40U, 0x00U});
    isotp_link_receive(link.get(), nullptr, 3U, 0U);
    EXPECT_EQ(isotp_link_invalid_frames(link.get()), 3U);
    EXPECT_TRUE(capture.messages.empty());
}

TEST_F(LinkReceive, MultiFrameUnlimitedBlock)
{
    const std::vector<uint8_t> message = pattern(20);
    const auto frames = segment(message);

    feed(link.get(), frames[0], 0U);
    ASSERT_EQ(capture.frames.size(), 1U);
    EXPECT_EQ(capture.frames[0].can_id, 0x7E0U);
    EXPECT_EQ(capture.frames[0].data, (std::vector<uint8_t>{0x30U, 0x00U, 0x00U}));
    feed(link.get(), frames[1], 10U);
    EXPECT_TRUE(capture.messages.empty());
    feed(link.get(), frames[2], 20U);
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], message);
    EXPECT_EQ(capture.frames.size(), 1U); /* BS = 0: one Flow Control only */
}

TEST_F(LinkReceive, EightByteMessage)
{
    const std::vector<uint8_t> message = pattern(8);
    for (const auto& frame : segment(message))
    {
        feed(link.get(), frame);
    }
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], message);
}

TEST_F(LinkReceive, FlowControlParametersAndBlocks)
{
    config.block_size = 2U;
    config.st_min_us = 20000U;
    reinit();
    const std::vector<uint8_t> message = pattern(40); /* FF + 5 CF */
    const auto frames = segment(message);

    for (const auto& frame : frames)
    {
        feed(link.get(), frame);
    }
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], message);
    /* after the FF, after CF 2 and after CF 4 */
    ASSERT_EQ(capture.frames.size(), 3U);
    for (const auto& fc : capture.frames)
    {
        EXPECT_EQ(fc.data, (std::vector<uint8_t>{0x30U, 0x02U, 0x14U}));
    }
}

TEST_F(LinkReceive, PaddedFlowControl)
{
    config.padding = true;
    config.padding_value = 0x55U;
    reinit();
    feed(link.get(), segment(pattern(20))[0]);
    EXPECT_EQ(capture.frames[0].data,
              (std::vector<uint8_t>{0x30U, 0x00U, 0x00U, 0x55U, 0x55U, 0x55U, 0x55U, 0x55U}));
}

TEST_F(LinkReceive, SequenceNumberWraps)
{
    const std::vector<uint8_t> message = pattern(6U + 7U * 20U);
    for (const auto& frame : segment(message))
    {
        feed(link.get(), frame);
    }
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], message);
}

TEST_F(LinkReceive, LongMessageEscapeSequence)
{
    const std::vector<uint8_t> message = pattern(4500); /* unit-test buffer is 5000 bytes */
    for (const auto& frame : segment(message))
    {
        feed(link.get(), frame);
    }
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], message);
}

TEST_F(LinkReceive, PaddedConsecutiveFramesAccepted)
{
    const std::vector<uint8_t> message = pattern(9);

    feed(link.get(), segment(message)[0]);
    feed(link.get(), {0x21U, 6, 7, 8, 0xCCU, 0xCCU, 0xCCU, 0xCCU});
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], message);
}

TEST_F(LinkReceive, MessageTooLongForBuffer)
{
    /* 6000 > 5000-byte unit-test buffer */
    feed(link.get(), {0x10U, 0x00U, 0x00U, 0x00U, 0x17U, 0x70U, 0x00U, 0x01U});
    ASSERT_EQ(capture.frames.size(), 1U);
    EXPECT_EQ(capture.frames[0].data, (std::vector<uint8_t>{0x32U, 0x00U, 0x00U}));
    EXPECT_EQ(capture.rx_errors, (std::vector<int>{ISOTP_N_BUFFER_OVFLW}));
    feed(link.get(), {0x21U, 1, 2, 3, 4, 5, 6, 7}); /* not receiving: ignored */
    EXPECT_TRUE(capture.messages.empty());
}

TEST_F(LinkReceive, FlowControlWriteFailsAfterFirstFrame)
{
    capture.write_results = {-1};
    const auto frames = segment(pattern(20));
    feed(link.get(), frames[0]);
    EXPECT_EQ(capture.rx_errors, (std::vector<int>{ISOTP_N_ERROR}));
    feed(link.get(), frames[1]);
    EXPECT_TRUE(capture.messages.empty());
}

TEST_F(LinkReceive, FlowControlWriteFailsBetweenBlocks)
{
    config.block_size = 1U;
    reinit();
    const auto frames = segment(pattern(30));
    feed(link.get(), frames[0]);
    capture.write_results = {ISOTP_BUSY};
    feed(link.get(), frames[1]);
    EXPECT_EQ(capture.rx_errors, (std::vector<int>{ISOTP_N_ERROR}));
    EXPECT_EQ(isotp_link_rx_result(link.get()), ISOTP_N_ERROR);
}

TEST_F(LinkReceive, WrongSequenceNumber)
{
    const auto frames = segment(pattern(30));
    feed(link.get(), frames[0]);
    feed(link.get(), frames[2]); /* SN 2 instead of 1 */
    EXPECT_EQ(capture.rx_errors, (std::vector<int>{ISOTP_N_WRONG_SN}));
    feed(link.get(), frames[1]);
    EXPECT_TRUE(capture.messages.empty());
}

TEST_F(LinkReceive, UnexpectedConsecutiveFrameIgnored)
{
    feed(link.get(), {0x21U, 1, 2, 3});
    EXPECT_TRUE(capture.rx_errors.empty());
    EXPECT_EQ(isotp_link_invalid_frames(link.get()), 0U);
}

TEST_F(LinkReceive, ShortLastConsecutiveFrameIgnored)
{
    const std::vector<uint8_t> message = pattern(9); /* FF + CF with 3 bytes */

    feed(link.get(), segment(message)[0]);
    feed(link.get(), {0x21U, 6, 7}); /* one byte missing */
    EXPECT_EQ(isotp_link_invalid_frames(link.get()), 1U);
    EXPECT_TRUE(capture.messages.empty());
    feed(link.get(), {0x21U, 6, 7, 8});
    ASSERT_EQ(capture.messages.size(), 1U);
}

TEST_F(LinkReceive, SingleFrameInterruptsReception)
{
    feed(link.get(), segment(pattern(20))[0]);
    feed(link.get(), {0x01U, 0x3EU});
    EXPECT_EQ(capture.rx_errors, (std::vector<int>{ISOTP_N_UNEXP_PDU}));
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], (std::vector<uint8_t>{0x3EU}));
    EXPECT_EQ(isotp_link_rx_result(link.get()), ISOTP_N_OK);
}

TEST_F(LinkReceive, FirstFrameRestartsReception)
{
    const std::vector<uint8_t> message = pattern(20);
    const auto frames = segment(message);

    feed(link.get(), frames[0]);
    feed(link.get(), frames[1]);
    for (const auto& frame : frames) /* the sender starts over */
    {
        feed(link.get(), frame);
    }
    EXPECT_EQ(capture.rx_errors, (std::vector<int>{ISOTP_N_UNEXP_PDU}));
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], message);
}

TEST_F(LinkReceive, ConsecutiveFrameTimeout)
{
    config.n_cr_timeout_us = 1000U;
    reinit();
    const auto frames = segment(pattern(30));
    feed(link.get(), frames[0], 0U);
    feed(link.get(), frames[1], 500U);
    isotp_link_poll(link.get(), 1499U);
    EXPECT_TRUE(capture.rx_errors.empty());
    isotp_link_poll(link.get(), 1500U);
    EXPECT_EQ(capture.rx_errors, (std::vector<int>{ISOTP_N_TIMEOUT_CR}));
    feed(link.get(), frames[2], 1600U);
    EXPECT_TRUE(capture.messages.empty());
}

TEST_F(LinkReceive, ErrorWithoutCallback)
{
    config.on_rx_error = nullptr;
    reinit();
    const auto frames = segment(pattern(30));
    feed(link.get(), frames[0]);
    feed(link.get(), frames[2]);
    EXPECT_EQ(isotp_link_rx_result(link.get()), ISOTP_N_WRONG_SN);
}

TEST_F(LinkReceive, CompleteWithoutHandler)
{
    config.on_message = nullptr;
    reinit();
    for (const auto& frame : segment(pattern(20)))
    {
        feed(link.get(), frame);
    }
    EXPECT_EQ(isotp_link_rx_result(link.get()), ISOTP_N_OK);
}

TEST_F(LinkReceive, HandlerAnswersOnSameLink)
{
    capture.reply_link = link.get();
    for (const auto& frame : segment(pattern(20)))
    {
        feed(link.get(), frame);
    }
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.reply_result, ISOTP_OK);
    EXPECT_EQ(capture.frames.back().data, (std::vector<uint8_t>{0x02U, 0x50U, 0x01U}));
}

TEST_F(LinkReceive, FullDuplex)
{
    const std::vector<uint8_t> out = pattern(20);
    const std::vector<uint8_t> in = pattern(30);
    const auto frames = segment(in);

    ASSERT_EQ(isotp_link_send(link.get(), out.data(), 20U, 0U), ISOTP_OK);
    feed(link.get(), frames[0]);
    feed(link.get(), {0x30U, 0x00U, 0x00U});
    for (size_t i = 1; i < frames.size(); i++)
    {
        isotp_link_poll(link.get(), 0U);
        feed(link.get(), frames[i]);
    }
    isotp_link_poll(link.get(), 0U);
    ASSERT_EQ(capture.messages.size(), 1U);
    EXPECT_EQ(capture.messages[0], in);
    EXPECT_EQ(capture.tx_done, (std::vector<int>{ISOTP_N_OK}));
}
