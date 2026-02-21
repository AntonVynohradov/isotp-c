

# File gtest\_isotp\_poll.cpp

[**File List**](files.md) **>** [**tests**](dir_59425e443f801f1f2fd8bbe4959a3ccf.md) **>** [**unit**](dir_7711e90abc3d4bd55823d9013a185d04.md) **>** [**gtest\_isotp\_poll.cpp**](gtest__isotp__poll_8cpp.md)

[Go to the documentation of this file](gtest__isotp__poll_8cpp.md)


```C++
/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: Unit tests for isotp_poll.
 *
 * Author:      Anton Vynohradov
 * Email:       avynohradov@systemfromscratch.com
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

TEST(IsotpPoll, SendsConsecutiveFrameAndCompletes)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_size = 8;
    link.send_offset = 7;
    link.send_sn = 1;
    link.send_bs_remain = ISOTP_INVALID_BS;
    link.send_st_min_us = 0;
    link.send_timer_st = 0;
    link.send_timer_bs = 1000;
    g_now_us = 1;

    link.send_buffer[7] = 0x5A;

    int ret_before = g_can_state.call_count;
    isotp_poll(&link);

    EXPECT_EQ(g_can_state.call_count, ret_before + 1);
    EXPECT_EQ(link.send_offset, 8u);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_IDLE);
    EXPECT_EQ(g_can_state.last_size, 2u);
}

TEST(IsotpPoll, ReceiveTimeoutSetsIdle)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[8] = {0};
    uint8_t recvbuf[8] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.receive_status = ISOTP_RECEIVE_STATUS_INPROGRESS;
    link.receive_timer_cr = 10;
    g_now_us = 11;

    isotp_poll(&link);

    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_IDLE);
    EXPECT_EQ(link.receive_protocol_result, ISOTP_PROTOCOL_RESULT_TIMEOUT_CR);
}

TEST(IsotpPoll, SendDoesNotStartWhenStMinNotElapsed)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_size = 8;
    link.send_offset = 0;
    link.send_sn = 1;
    link.send_bs_remain = 1;
    link.send_st_min_us = 50;
    link.send_timer_st = 100;
    link.send_timer_bs = 1000;
    g_now_us = 90;

    isotp_poll(&link);

    EXPECT_EQ(g_can_state.call_count, 0);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_INPROGRESS);
    EXPECT_EQ(link.send_offset, 0u);
}

TEST(IsotpPoll, SendNoSpaceDoesNotChangeState)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_size = 9;
    link.send_offset = 0;
    link.send_sn = 1;
    link.send_bs_remain = ISOTP_INVALID_BS;
    link.send_st_min_us = 0;
    link.send_timer_st = 0;
    link.send_timer_bs = 500;
    g_now_us = 1;

    g_can_state.return_value = ISOTP_RET_NOSPACE;

    isotp_poll(&link);

    EXPECT_EQ(g_can_state.call_count, 1);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_INPROGRESS);
    EXPECT_EQ(link.send_offset, 0u);
    EXPECT_EQ(link.send_timer_bs, 500u);
}

TEST(IsotpPoll, SendErrorSetsSendStatusError)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_size = 9;
    link.send_offset = 0;
    link.send_sn = 1;
    link.send_bs_remain = ISOTP_INVALID_BS;
    link.send_st_min_us = 0;
    link.send_timer_st = 0;
    link.send_timer_bs = 500;
    g_now_us = 1;

    g_can_state.return_value = ISOTP_RET_ERROR;

    isotp_poll(&link);

    EXPECT_EQ(g_can_state.call_count, 1);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_ERROR);
}

TEST(IsotpPoll, SendDecrementsBlockSizeRemain)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_size = 9;
    link.send_offset = 0;
    link.send_sn = 1;
    link.send_bs_remain = 2;
    link.send_st_min_us = 0;
    link.send_timer_st = 0;
    link.send_timer_bs = 1000;
    g_now_us = 10;

    isotp_poll(&link);

    EXPECT_EQ(g_can_state.call_count, 1);
    EXPECT_EQ(link.send_offset, 7u);
    EXPECT_EQ(link.send_bs_remain, 1u);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_INPROGRESS);
    EXPECT_EQ(link.send_timer_bs, g_now_us + ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US);
}

TEST(IsotpPoll, SendTimeoutSetsError)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_bs_remain = 0;
    link.send_timer_bs = 10;
    g_now_us = 11;

    isotp_poll(&link);

    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_ERROR);
    EXPECT_EQ(link.send_protocol_result, ISOTP_PROTOCOL_RESULT_TIMEOUT_BS);
}

TEST(IsotpPoll, ReceiveDoesNotTimeoutWhenTimerZero)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[8] = {0};
    uint8_t recvbuf[8] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.receive_status = ISOTP_RECEIVE_STATUS_INPROGRESS;
    link.receive_timer_cr = 0;
    link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_OK;
    g_now_us = 100;

    isotp_poll(&link);

    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_INPROGRESS);
    EXPECT_EQ(link.receive_protocol_result, ISOTP_PROTOCOL_RESULT_OK);
}
```


