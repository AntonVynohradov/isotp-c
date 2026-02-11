/*******************************************************************************
# * ISO-TP-C: ISO 15765-2 Protocol Implementation
# *
# * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
# * Description: Unit tests for isotp_receive.
# *
# * Author:      Anton Vynohradov
# * Email:       avynohradovair@gmail.com
# * 
# * License:     MIT License
# *
# * Copyright (c) 2026 Anton Vynohradov
# *
# * Permission is hereby granted, free of charge, to any person obtaining a copy
# * of this software and associated documentation files (the "Software"), to deal
# * in the Software without restriction, including without limitation the rights
# * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# * copies of the Software, and to permit persons to whom the Software is
# * furnished to do so, subject to the following conditions:
# *
# * The above copyright notice and this permission notice shall be included in
# * all copies or substantial portions of the Software.
# *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# * THE SOFTWARE.
# *
# * SPDX-License-Identifier: MIT
# ******************************************************************************/

#include <gtest/gtest.h>

#include "isotp_test_support.h"

TEST(IsotpReceive, NoDataReturnsNoData)
{
    IsoTpLink link;
    uint8_t sendbuf[8] = {0};
    uint8_t recvbuf[8] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    uint8_t payload[8] = {0};
    uint32_t out_size = 0;

    int ret = isotp_receive(&link, payload, sizeof(payload), &out_size);

    EXPECT_EQ(ret, ISOTP_RET_NO_DATA);
    EXPECT_EQ(out_size, 0u);
}

TEST(IsotpReceive, CopiesPayloadAndResetsState)
{
    IsoTpLink link;
    uint8_t sendbuf[8] = {0};
    uint8_t recvbuf[8] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.receive_status = ISOTP_RECEIVE_STATUS_FULL;
    link.receive_size = 3;
    link.receive_buffer[0] = 0xA1;
    link.receive_buffer[1] = 0xB2;
    link.receive_buffer[2] = 0xC3;

    uint8_t payload[8] = {0};
    uint32_t out_size = 0;

    int ret = isotp_receive(&link, payload, sizeof(payload), &out_size);

    EXPECT_EQ(ret, ISOTP_RET_OK);
    EXPECT_EQ(out_size, 3u);
    EXPECT_EQ(payload[0], 0xA1);
    EXPECT_EQ(payload[1], 0xB2);
    EXPECT_EQ(payload[2], 0xC3);
    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_IDLE);
}
