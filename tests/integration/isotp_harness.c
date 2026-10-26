/**
 * @file isotp_harness.c
 * @brief Integration-test harness: one ISO-TP link with a recording CAN writer (loaded with
 * ctypes).
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

/*
 * Built by tests/integration/conftest.py together with src/ into a shared
 * library. Test code only: the static state below is allowed here, not in the
 * library.
 */

#include <string.h>

#include "isotp.h"

#define HARNESS_MAX_FRAMES 4096U
#define HARNESS_MAX_RESULTS 64U

static struct isotp_link harness_link;
static uint8_t harness_frames[HARNESS_MAX_FRAMES][ISOTP_CAN_DL];
static uint8_t harness_lengths[HARNESS_MAX_FRAMES];
static uint32_t harness_ids[HARNESS_MAX_FRAMES];
static unsigned harness_frame_count;
static int harness_fail_code;
static unsigned harness_fail_count;
static uint8_t harness_message[ISOTP_RX_BUFFER_SIZE];
static uint32_t harness_message_length;
static unsigned harness_message_count;
static int harness_tx_done[HARNESS_MAX_RESULTS];
static unsigned harness_tx_done_count;
static int harness_rx_error_log[HARNESS_MAX_RESULTS];
static unsigned harness_rx_error_count;

static int harness_write(void* context, uint32_t can_id, const uint8_t* data, size_t length)
{
    int result = 0;

    (void) context;
    if (harness_fail_count > 0U)
    {
        harness_fail_count--;
        result = harness_fail_code;
    }
    else if (harness_frame_count < HARNESS_MAX_FRAMES)
    {
        (void) memcpy(harness_frames[harness_frame_count], data, length);
        harness_lengths[harness_frame_count] = (uint8_t) length;
        harness_ids[harness_frame_count] = can_id;
        harness_frame_count++;
    }
    else
    {
        result = -100;
    }
    return result;
}

static void harness_on_message(const uint8_t* data, uint32_t length, void* context)
{
    (void) context;
    (void) memcpy(harness_message, data, length);
    harness_message_length = length;
    harness_message_count++;
}

static void harness_on_tx_done(int n_result, void* context)
{
    (void) context;
    if (harness_tx_done_count < HARNESS_MAX_RESULTS)
    {
        harness_tx_done[harness_tx_done_count] = n_result;
        harness_tx_done_count++;
    }
}

static void harness_on_rx_error(int n_result, void* context)
{
    (void) context;
    if (harness_rx_error_count < HARNESS_MAX_RESULTS)
    {
        harness_rx_error_log[harness_rx_error_count] = n_result;
        harness_rx_error_count++;
    }
}

int harness_init(uint32_t tx_id, uint8_t block_size, uint32_t st_min_us, uint32_t n_bs_us,
                 uint32_t n_cr_us, uint8_t wft_max, int padding)
{
    struct isotp_link_config config;

    harness_frame_count = 0U;
    harness_fail_count = 0U;
    harness_message_count = 0U;
    harness_message_length = 0U;
    harness_tx_done_count = 0U;
    harness_rx_error_count = 0U;

    isotp_link_config_default(&config);
    config.tx_id = tx_id;
    config.write = harness_write;
    config.on_message = harness_on_message;
    config.on_tx_done = harness_on_tx_done;
    config.on_rx_error = harness_on_rx_error;
    config.block_size = block_size;
    config.st_min_us = st_min_us;
    config.n_bs_timeout_us = n_bs_us;
    config.n_cr_timeout_us = n_cr_us;
    config.wft_max = wft_max;
    config.padding = (padding != 0);
    return isotp_link_init(&harness_link, &config);
}

unsigned harness_buffer_size(void)
{
    return (unsigned) ISOTP_RX_BUFFER_SIZE;
}

int harness_send(const uint8_t* data, uint32_t length, uint32_t now_us)
{
    return isotp_link_send(&harness_link, data, length, now_us);
}

int harness_send_functional(uint32_t can_id, const uint8_t* data, uint32_t length)
{
    return isotp_link_send_functional(&harness_link, can_id, data, length);
}

void harness_receive(const uint8_t* data, size_t length, uint32_t now_us)
{
    isotp_link_receive(&harness_link, data, length, now_us);
}

void harness_poll(uint32_t now_us)
{
    isotp_link_poll(&harness_link, now_us);
}

void harness_fail_writes(int code, unsigned count)
{
    harness_fail_code = code;
    harness_fail_count = count;
}

unsigned harness_frames_written(void)
{
    return harness_frame_count;
}

unsigned harness_frame(unsigned index, uint8_t* out, uint32_t* can_id)
{
    unsigned length = 0U;

    if (index < harness_frame_count)
    {
        length = harness_lengths[index];
        (void) memcpy(out, harness_frames[index], length);
        *can_id = harness_ids[index];
    }
    return length;
}

unsigned harness_messages(void)
{
    return harness_message_count;
}

uint32_t harness_last_message(uint8_t* out, uint32_t capacity)
{
    uint32_t length = (harness_message_length < capacity) ? harness_message_length : capacity;

    (void) memcpy(out, harness_message, length);
    return harness_message_length;
}

unsigned harness_tx_results(int* out, unsigned capacity)
{
    unsigned i;

    for (i = 0U; (i < harness_tx_done_count) && (i < capacity); i++)
    {
        out[i] = harness_tx_done[i];
    }
    return harness_tx_done_count;
}

unsigned harness_rx_errors(int* out, unsigned capacity)
{
    unsigned i;

    for (i = 0U; (i < harness_rx_error_count) && (i < capacity); i++)
    {
        out[i] = harness_rx_error_log[i];
    }
    return harness_rx_error_count;
}

int harness_tx_busy(void)
{
    return isotp_link_tx_busy(&harness_link) ? 1 : 0;
}

uint32_t harness_invalid_frames(void)
{
    return isotp_link_invalid_frames(&harness_link);
}
