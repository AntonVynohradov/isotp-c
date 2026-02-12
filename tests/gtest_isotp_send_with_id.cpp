/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: Unit tests for isotp_send_with_id.
 *
 * Author:      Anton Vynohradov
 * Email:       avynohradovair@gmail.com
 *
 * License:     MIT License
 *
 * Copyright (c) 2026 Anton Vynohradov
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
 ******************************************************************************/

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <gtest/gtest.h>

#include "isotp_test_support.h"

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

/* Macros and constants for this test file */

/* ==============================================================================
 * PRIVATE TYPE DEFINITIONS
 * =============================================================================*/

/* Local type definitions */

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

/* static variables */

/* ==============================================================================
 * PRIVATE FUNCTION DECLARATIONS (static)
 * =============================================================================*/

/* static helpers */

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

/* static helper implementations */

/* ==============================================================================
 * UNIT TEST IMPLEMENTATIONS
 * =============================================================================*/

TEST(IsotpSendWithId, NullLinkReturnsError)
{
    const uint8_t payload[1] = {0x00};

    int ret = isotp_send_with_id(NULL, 0x123u, payload, sizeof(payload));

    EXPECT_EQ(ret, ISOTP_RET_ERROR);
}

TEST(IsotpSendWithId, OversizeReturnsOverflow)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[4] = {0};
    uint8_t recvbuf[4] = {0};
    const uint8_t payload[5] = {1, 2, 3, 4, 5};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    int ret = isotp_send_with_id(&link, 0x123u, payload, sizeof(payload));

    EXPECT_EQ(ret, ISOTP_RET_OVERFLOW);
    EXPECT_EQ(g_can_state.call_count, 0);
}

TEST(IsotpSendWithId, InProgressReturnsInProgress)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[8] = {0};
    uint8_t recvbuf[8] = {0};
    const uint8_t payload[3] = {1, 2, 3};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));
    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;

    int ret = isotp_send_with_id(&link, 0x123u, payload, sizeof(payload));

    EXPECT_EQ(ret, ISOTP_RET_INPROGRESS);
    EXPECT_EQ(g_can_state.call_count, 0);
}

TEST(IsotpSendWithId, SingleFrameSendsPayload)
{
    GTEST_SKIP();
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[8] = {0};
    uint8_t recvbuf[8] = {0};
    const uint8_t payload[3] = {0xAA, 0xBB, 0xCC};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    int ret = isotp_send_with_id(&link, 0x456u, payload, sizeof(payload));

    EXPECT_EQ(ret, ISOTP_RET_OK);
    EXPECT_EQ(g_can_state.call_count, 1);
    EXPECT_EQ(g_can_state.last_id, 0x456u);
    EXPECT_EQ(g_can_state.last_size, 4u);

#if defined(ISOTP_BYTE_ORDER_LITTLE_ENDIAN)
    EXPECT_EQ(g_can_state.last_data[0], static_cast<uint8_t>((ISOTP_PCI_TYPE_SINGLE << 4) | 0x03));
#else
    EXPECT_EQ(g_can_state.last_data[0], static_cast<uint8_t>((0x03 << 4) | ISOTP_PCI_TYPE_SINGLE));
#endif
    EXPECT_EQ(g_can_state.last_data[1], payload[0]);
    EXPECT_EQ(g_can_state.last_data[2], payload[1]);
    EXPECT_EQ(g_can_state.last_data[3], payload[2]);
}

TEST(IsotpSendWithId, MultiFrameFirstFrameStartsSend)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};
    const uint8_t payload[8] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));
    g_now_us = 42;

    int ret = isotp_send_with_id(&link, 0x456u, payload, sizeof(payload));

    EXPECT_EQ(ret, ISOTP_RET_OK);
    EXPECT_EQ(g_can_state.call_count, 1);
    EXPECT_EQ(g_can_state.last_id, 0x456u);
    EXPECT_EQ(g_can_state.last_size, 8u);

    IsoTpCanMessage expected;
    std::memset(&expected, 0, sizeof(expected));
    expected.as.first_frame_short.type = ISOTP_PCI_TYPE_FIRST_FRAME;
    expected.as.first_frame_short.FF_DL_high = 0;
    expected.as.first_frame_short.FF_DL_low = 8;
    std::memcpy(expected.as.first_frame_short.data, payload,
                sizeof(expected.as.first_frame_short.data));

    EXPECT_EQ(std::memcmp(g_can_state.last_data, expected.as.data_array.ptr, 8), 0);
    EXPECT_EQ(link.send_offset, 6u);
    EXPECT_EQ(link.send_sn, 1u);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_INPROGRESS);
    EXPECT_EQ(link.send_timer_st, g_now_us);
    EXPECT_EQ(link.send_timer_bs, g_now_us + ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US);
}

TEST(IsotpSendWithId, MultiFrameFirstFrameSendErrorDoesNotStart)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};
    const uint8_t payload[8] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));
    g_can_state.return_value = ISOTP_RET_ERROR;

    int ret = isotp_send_with_id(&link, 0x456u, payload, sizeof(payload));

    EXPECT_EQ(ret, ISOTP_RET_ERROR);
    EXPECT_EQ(g_can_state.call_count, 1);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_IDLE);
    EXPECT_EQ(link.send_offset, 0u);
}
