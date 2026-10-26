/**
 * @file isotp_test_mocks.cpp
 * @brief Test support: implementation of the mocks.
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

int capture_write(void* context, uint32_t can_id, const uint8_t* data, size_t length)
{
    Capture* capture = static_cast<Capture*>(context);
    int result = 0;

    if (!capture->write_results.empty())
    {
        result = capture->write_results.front();
        capture->write_results.erase(capture->write_results.begin());
    }
    if (result == 0)
    {
        capture->frames.push_back({can_id, std::vector<uint8_t>(data, data + length)});
    }
    return result;
}

void record_message(const uint8_t* data, uint32_t length, void* context)
{
    Capture* capture = static_cast<Capture*>(context);

    capture->messages.emplace_back(data, data + length);
    if (capture->reply_link != nullptr)
    {
        const uint8_t answer[] = {0x50U, 0x01U};
        capture->reply_result = isotp_link_send(capture->reply_link, answer, sizeof(answer), 0U);
    }
}

void record_tx_done(int n_result, void* context)
{
    static_cast<Capture*>(context)->tx_done.push_back(n_result);
}

void record_rx_error(int n_result, void* context)
{
    static_cast<Capture*>(context)->rx_errors.push_back(n_result);
}

struct isotp_link_config make_config(Capture* capture)
{
    struct isotp_link_config config;

    isotp_link_config_default(&config);
    config.tx_id = 0x7E0U;
    config.write = capture_write;
    config.write_context = capture;
    config.on_message = record_message;
    config.on_tx_done = record_tx_done;
    config.on_rx_error = record_rx_error;
    config.app_context = capture;
    config.block_size = 0U;
    config.st_min_us = 0U;
    return config;
}

std::vector<uint8_t> pattern(size_t length)
{
    std::vector<uint8_t> bytes(length);

    for (size_t i = 0; i < length; i++)
    {
        bytes[i] = static_cast<uint8_t>(i);
    }
    return bytes;
}

void feed(struct isotp_link* link, std::vector<uint8_t> frame, uint32_t now_us)
{
    isotp_link_receive(link, frame.data(), frame.size(), now_us);
}
