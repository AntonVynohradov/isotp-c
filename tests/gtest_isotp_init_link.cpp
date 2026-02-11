/*******************************************************************************
# * ISO-TP-C: ISO 15765-2 Protocol Implementation
# *
# * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
# * Description: Unit tests for isotp_init_link.
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

TEST(IsotpInitLink, InitializesFields)
{
    IsoTpLink link;
    uint8_t sendbuf[32] = {0};
    uint8_t recvbuf[48] = {0};

    std::memset(&link, 0xA5, sizeof(link));

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    EXPECT_EQ(link.send_arbitration_id, 0x123u);
    EXPECT_EQ(link.send_buffer, sendbuf);
    EXPECT_EQ(link.send_buf_size, sizeof(sendbuf));
    EXPECT_EQ(link.receive_buffer, recvbuf);
    EXPECT_EQ(link.receive_buf_size, sizeof(recvbuf));
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_IDLE);
    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_IDLE);
    EXPECT_EQ(link.send_size, 0u);
    EXPECT_EQ(link.receive_size, 0u);
    EXPECT_EQ(link.send_offset, 0u);
    EXPECT_EQ(link.receive_offset, 0u);
}
