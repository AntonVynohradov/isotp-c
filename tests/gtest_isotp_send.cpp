/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: Unit tests for isotp_send.
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

TEST(IsotpSend, UsesLinkArbitrationId)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};
    const uint8_t payload[3] = {0x11, 0x22, 0x33};

    isotp_init_link(&link, 0x7DFu, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    int ret = isotp_send(&link, payload, sizeof(payload));

    EXPECT_EQ(ret, ISOTP_RET_OK);
    EXPECT_EQ(g_can_state.call_count, 1);
    EXPECT_EQ(g_can_state.last_id, 0x7DFu);
}

TEST(IsotpSend, OversizeReturnsOverflow)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[4] = {0};
    uint8_t recvbuf[4] = {0};
    const uint8_t payload[5] = {1, 2, 3, 4, 5};

    isotp_init_link(&link, 0x7DFu, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    int ret = isotp_send(&link, payload, sizeof(payload));

    EXPECT_EQ(ret, ISOTP_RET_OVERFLOW);
    EXPECT_EQ(g_can_state.call_count, 0);
}

TEST(IsotpSend, InProgressReturnsInProgress)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[8] = {0};
    uint8_t recvbuf[8] = {0};
    const uint8_t payload[3] = {1, 2, 3};

    isotp_init_link(&link, 0x7DFu, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));
    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;

    int ret = isotp_send(&link, payload, sizeof(payload));

    EXPECT_EQ(ret, ISOTP_RET_INPROGRESS);
    EXPECT_EQ(g_can_state.call_count, 0);
}
