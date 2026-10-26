/**
 * @file isotp_test_support.h
 * @brief Test support: CAN frame capture, callback recorders, link fixture.
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

#ifndef ISOTP_TEST_SUPPORT_H
#define ISOTP_TEST_SUPPORT_H

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

extern "C"
{
#include "isotp.h"
}

/* ==============================================================================
 * TYPES
 * =============================================================================*/

/** One frame written by the link. */
struct CapturedFrame
{
    uint32_t can_id;
    std::vector<uint8_t> data;
};

/** Everything the link reported through its callbacks. */
struct Capture
{
    std::vector<CapturedFrame> frames;          /**< Frames passed to the writer. */
    std::vector<int> write_results;             /**< Results returned by the next writes (FIFO). */
    std::vector<std::vector<uint8_t>> messages; /**< Messages passed to on_message. */
    std::vector<int> tx_done;                   /**< Results passed to on_tx_done. */
    std::vector<int> rx_errors;                 /**< Results passed to on_rx_error. */
    struct isotp_link* reply_link = nullptr;    /**< When set, on_message answers on this link. */
    int reply_result = 0;                       /**< Result of that answer. */
};

/* ==============================================================================
 * MOCKS
 * =============================================================================*/

int capture_write(void* context, uint32_t can_id, const uint8_t* data, size_t length);
void record_message(const uint8_t* data, uint32_t length, void* context);
void record_tx_done(int n_result, void* context);
void record_rx_error(int n_result, void* context);

/** Default configuration wired to @p capture: tx_id 0x7E0, BS 0, STmin 0. */
struct isotp_link_config make_config(Capture* capture);

/** Bytes 0, 1, 2, ... of the given length. */
std::vector<uint8_t> pattern(size_t length);

/** Feed one frame given as bytes. */
void feed(struct isotp_link* link, std::vector<uint8_t> frame, uint32_t now_us = 0U);

/** Link state is large (two message buffers): keep it on the heap. */
struct LinkTest : public ::testing::Test
{
    Capture capture;
    std::unique_ptr<struct isotp_link> link{new struct isotp_link()};
    struct isotp_link_config config
    {
    };

    void SetUp() override
    {
        config = make_config(&capture);
        ASSERT_EQ(isotp_link_init(link.get(), &config), ISOTP_OK);
    }

    void reinit() { ASSERT_EQ(isotp_link_init(link.get(), &config), ISOTP_OK); }
};

#endif /* ISOTP_TEST_SUPPORT_H */
