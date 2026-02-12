/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: Unit tests for isotp_on_can_message.
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

TEST(IsotpOnCanMessage, SingleFrameSetsReceiveBuffer)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.single_frame.type = ISOTP_PCI_TYPE_SINGLE;
    message.as.single_frame.SF_DL = 2;
    message.as.single_frame.data[0] = 0x11;
    message.as.single_frame.data[1] = 0x22;

    isotp_on_can_message(&link, message.as.data_array.ptr, 3);

    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_FULL);
    EXPECT_EQ(link.receive_size, 2u);
    EXPECT_EQ(link.receive_buffer[0], 0x11);
    EXPECT_EQ(link.receive_buffer[1], 0x22);
}

TEST(IsotpOnCanMessage, InvalidLengthIsIgnored)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.receive_status = ISOTP_RECEIVE_STATUS_INPROGRESS;
    link.receive_protocol_result = ISOTP_PROTOCOL_RESULT_OK;

    uint8_t short_data[1] = {0};
    uint8_t long_data[9] = {0};

    isotp_on_can_message(&link, short_data, 1);
    isotp_on_can_message(&link, long_data, 9);

    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_INPROGRESS);
    EXPECT_EQ(link.receive_protocol_result, ISOTP_PROTOCOL_RESULT_OK);
    EXPECT_EQ(g_can_state.call_count, 0);
}

TEST(IsotpOnCanMessage, SingleFrameInProgressLengthErrorSetsUnexpected)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.receive_status = ISOTP_RECEIVE_STATUS_INPROGRESS;

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.single_frame.type = ISOTP_PCI_TYPE_SINGLE;
    message.as.single_frame.SF_DL = 0;

    isotp_on_can_message(&link, message.as.data_array.ptr, 2);

    EXPECT_EQ(link.receive_protocol_result, ISOTP_PROTOCOL_RESULT_UNEXP_PDU);
    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_INPROGRESS);
}

TEST(IsotpOnCanMessage, FirstFrameOverflowSendsFlowControlOverflow)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[4] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.first_frame_short.type = ISOTP_PCI_TYPE_FIRST_FRAME;
    message.as.first_frame_short.FF_DL_high = 0;
    message.as.first_frame_short.FF_DL_low = 10;

    isotp_on_can_message(&link, message.as.data_array.ptr, 8);

    EXPECT_EQ(link.receive_protocol_result, ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW);
    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_IDLE);
    EXPECT_EQ(g_can_state.call_count, 1);

    IsoTpCanMessage expected;
    std::memset(&expected, 0, sizeof(expected));
    expected.as.flow_control.type = ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME;
    expected.as.flow_control.FS = PCI_FLOW_STATUS_OVERFLOW;
    expected.as.flow_control.BS = 0;
    expected.as.flow_control.STmin = 0;

    EXPECT_EQ(g_can_state.last_size, 3u);
    EXPECT_EQ(std::memcmp(g_can_state.last_data, expected.as.data_array.ptr, 3), 0);
}

TEST(IsotpOnCanMessage, FirstFrameOkStartsReceptionAndSendsFlowControl)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[32] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    g_now_us = 100;

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.first_frame_short.type = ISOTP_PCI_TYPE_FIRST_FRAME;
    message.as.first_frame_short.FF_DL_high = 0;
    message.as.first_frame_short.FF_DL_low = 10;
    message.as.first_frame_short.data[0] = 0xA1;
    message.as.first_frame_short.data[1] = 0xB2;

    isotp_on_can_message(&link, message.as.data_array.ptr, 8);

    EXPECT_EQ(link.receive_protocol_result, ISOTP_PROTOCOL_RESULT_OK);
    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_INPROGRESS);
    EXPECT_EQ(link.receive_size, 10u);
    EXPECT_EQ(link.receive_offset, 6u);
    EXPECT_EQ(link.receive_sn, 1u);
    EXPECT_EQ(link.receive_buffer[0], 0xA1);
    EXPECT_EQ(link.receive_buffer[1], 0xB2);
    EXPECT_EQ(link.receive_timer_cr, g_now_us + ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US);

    EXPECT_EQ(g_can_state.call_count, 1);

    IsoTpCanMessage expected;
    std::memset(&expected, 0, sizeof(expected));
    expected.as.flow_control.type = ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME;
    expected.as.flow_control.FS = PCI_FLOW_STATUS_CONTINUE;
    expected.as.flow_control.BS = ISO_TP_DEFAULT_BLOCK_SIZE;
    expected.as.flow_control.STmin = 0;

    EXPECT_EQ(g_can_state.last_size, 3u);
    EXPECT_EQ(std::memcmp(g_can_state.last_data, expected.as.data_array.ptr, 3), 0);
}

TEST(IsotpOnCanMessage, ConsecutiveFrameUnexpectedWhenIdle)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.consecutive_frame.type = ISOTP_PCI_TYPE_CONSECUTIVE_FRAME;
    message.as.consecutive_frame.SN = 1;
    message.as.consecutive_frame.data[0] = 0x5A;

    isotp_on_can_message(&link, message.as.data_array.ptr, 2);

    EXPECT_EQ(link.receive_protocol_result, ISOTP_PROTOCOL_RESULT_UNEXP_PDU);
    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_IDLE);
}

TEST(IsotpOnCanMessage, ConsecutiveFrameWrongSnSetsError)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.receive_status = ISOTP_RECEIVE_STATUS_INPROGRESS;
    link.receive_sn = 2;

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.consecutive_frame.type = ISOTP_PCI_TYPE_CONSECUTIVE_FRAME;
    message.as.consecutive_frame.SN = 1;
    message.as.consecutive_frame.data[0] = 0x6B;

    isotp_on_can_message(&link, message.as.data_array.ptr, 2);

    EXPECT_EQ(link.receive_protocol_result, ISOTP_PROTOCOL_RESULT_WRONG_SN);
    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_IDLE);
}

TEST(IsotpOnCanMessage, ConsecutiveFrameCompletesReceive)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    link.receive_status = ISOTP_RECEIVE_STATUS_INPROGRESS;
    link.receive_size = 8;
    link.receive_offset = 7;
    link.receive_sn = 1;
    link.receive_buffer[7] = 0;

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.consecutive_frame.type = ISOTP_PCI_TYPE_CONSECUTIVE_FRAME;
    message.as.consecutive_frame.SN = 1;
    message.as.consecutive_frame.data[0] = 0xAB;

    isotp_on_can_message(&link, message.as.data_array.ptr, 2);

    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_FULL);
    EXPECT_EQ(link.receive_offset, 8u);
    EXPECT_EQ(link.receive_buffer[7], 0xAB);
}

TEST(IsotpOnCanMessage, ConsecutiveFrameSendsFlowControlWhenBlockSizeExhausted)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[32] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    g_now_us = 50;

    link.receive_status = ISOTP_RECEIVE_STATUS_INPROGRESS;
    link.receive_size = 20;
    link.receive_offset = 0;
    link.receive_sn = 1;
    link.receive_bs_count = 1;

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.consecutive_frame.type = ISOTP_PCI_TYPE_CONSECUTIVE_FRAME;
    message.as.consecutive_frame.SN = 1;
    message.as.consecutive_frame.data[0] = 0x10;

    isotp_on_can_message(&link, message.as.data_array.ptr, 8);

    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_INPROGRESS);
    EXPECT_EQ(link.receive_offset, 7u);
    EXPECT_EQ(link.receive_bs_count, ISO_TP_DEFAULT_BLOCK_SIZE);
    EXPECT_EQ(link.receive_timer_cr, g_now_us + ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US);
    EXPECT_EQ(g_can_state.call_count, 1);

    IsoTpCanMessage expected;
    std::memset(&expected, 0, sizeof(expected));
    expected.as.flow_control.type = ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME;
    expected.as.flow_control.FS = PCI_FLOW_STATUS_CONTINUE;
    expected.as.flow_control.BS = ISO_TP_DEFAULT_BLOCK_SIZE;
    expected.as.flow_control.STmin = 0;

    EXPECT_EQ(g_can_state.last_size, 3u);
    EXPECT_EQ(std::memcmp(g_can_state.last_data, expected.as.data_array.ptr, 3), 0);
}

TEST(IsotpOnCanMessage, FlowControlIgnoredWhenSendIdle)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.flow_control.type = ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME;
    message.as.flow_control.FS = PCI_FLOW_STATUS_CONTINUE;

    isotp_on_can_message(&link, message.as.data_array.ptr, 3);

    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_IDLE);
    EXPECT_EQ(g_can_state.call_count, 0);
}

TEST(IsotpOnCanMessage, FlowControlOverflowSetsSendError)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    g_now_us = 200;

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_protocol_result = ISOTP_PROTOCOL_RESULT_OK;

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.flow_control.type = ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME;
    message.as.flow_control.FS = PCI_FLOW_STATUS_OVERFLOW;

    isotp_on_can_message(&link, message.as.data_array.ptr, 3);

    EXPECT_EQ(link.send_protocol_result, ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_ERROR);
    EXPECT_EQ(link.send_timer_bs, g_now_us + ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US);
}

TEST(IsotpOnCanMessage, FlowControlWaitExceedsSetsError)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    g_now_us = 300;

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_wtf_count = ISO_TP_MAX_WFT_NUMBER;
    link.send_protocol_result = ISOTP_PROTOCOL_RESULT_OK;

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.flow_control.type = ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME;
    message.as.flow_control.FS = PCI_FLOW_STATUS_WAIT;

    isotp_on_can_message(&link, message.as.data_array.ptr, 3);

    EXPECT_EQ(link.send_protocol_result, ISOTP_PROTOCOL_RESULT_WFT_OVRN);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_ERROR);
    EXPECT_EQ(link.send_timer_bs, g_now_us + ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US);
}

TEST(IsotpOnCanMessage, FlowControlContinueBsZeroSetsInvalidBsAndStmin)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    g_now_us = 400;

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_wtf_count = 2;

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.flow_control.type = ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME;
    message.as.flow_control.FS = PCI_FLOW_STATUS_CONTINUE;
    message.as.flow_control.BS = 0;
    message.as.flow_control.STmin = 5;

    isotp_on_can_message(&link, message.as.data_array.ptr, 3);

    EXPECT_EQ(link.send_bs_remain, ISOTP_INVALID_BS);
    EXPECT_EQ(link.send_st_min_us, 5000u);
    EXPECT_EQ(link.send_wtf_count, 0);
    EXPECT_EQ(link.send_timer_bs, g_now_us + ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US);
}

TEST(IsotpOnCanMessage, FlowControlContinueBsNonZeroSetsBs)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    g_now_us = 450;

    link.send_status = ISOTP_SEND_STATUS_INPROGRESS;
    link.send_bs_remain = 0;

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.flow_control.type = ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME;
    message.as.flow_control.FS = PCI_FLOW_STATUS_CONTINUE;
    message.as.flow_control.BS = 4;
    message.as.flow_control.STmin = 0;

    isotp_on_can_message(&link, message.as.data_array.ptr, 3);

    EXPECT_EQ(link.send_bs_remain, 4u);
    EXPECT_EQ(link.send_st_min_us, 0u);
    EXPECT_EQ(link.send_timer_bs, g_now_us + ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US);
}

TEST(IsotpOnCanMessage, UnknownTypeIgnored)
{
    reset_mocks();

    IsoTpLink link;
    uint8_t sendbuf[16] = {0};
    uint8_t recvbuf[16] = {0};

    isotp_init_link(&link, 0x123u, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    IsoTpCanMessage message;
    std::memset(&message, 0, sizeof(message));
    message.as.common.type = 0xF;

    isotp_on_can_message(&link, message.as.data_array.ptr, 2);

    EXPECT_EQ(link.receive_status, ISOTP_RECEIVE_STATUS_IDLE);
    EXPECT_EQ(link.send_status, ISOTP_SEND_STATUS_IDLE);
}
