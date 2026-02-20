/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: The mock CAN driver implementation for testing purposes.
 *              This file provides a simple in-memory queue to simulate CAN communication,
 *              allowing for testing of the ISO-TP implementation without requiring actual CAN
 *              hardware. It supports basic send and receive operations, as well as optional frame
 *              dropping and artificial delays.
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

#ifndef _WIN32
#define _POSIX_C_SOURCE 199309L
#endif

#include "mock_can.h"
#include "can_driver.h"
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define MOCK_QUEUE_SIZE 256  ///< Size of the internal queue for simulating CAN frames

#define CAN_MAX_DLEN 8  ///< Maximum data length for CAN frames

/* ==============================================================================
 * PRIVATE TYPE DEFINITIONS
 * =============================================================================*/

/**
 * @brief Structure representing a CAN frame in the mock driver.
 *
 */
typedef struct
{
    uint32_t id;
    uint8_t data[CAN_MAX_DLEN];
    uint8_t len;

} can_frame_t;

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

static can_frame_t queue[MOCK_QUEUE_SIZE];  ///< Internal queue to hold CAN frames for simulation

static int head = 0;  ///< Index of the head of the queue (next frame to receive)
static int tail = 0;  ///< Index of the tail of the queue (next position to send a frame)

static int drop_enabled = 0;  ///< Flag to enable or disable frame dropping in the mock driver
static int delay_ms = 0;  ///< Artificial delay in milliseconds for send operations (0 for no delay)
static int disable_fc = 0;  ///< Flag to disable FlowControl frames in the mock driver

/* ==============================================================================
 * PRIVATE FUNCTION DECLARATIONS (static)
 * =============================================================================*/

/**
 * @brief Sleep for a specified number of milliseconds.
 *
 * @param ms Number of milliseconds to sleep.
 */
static void sleep_ms(int ms);

/**
 * @brief Mock implementation of CAN receive function.
 *
 * @param id Pointer to store the received CAN ID.
 * @param data Pointer to store the received CAN data.
 * @param len Pointer to store the length of the received CAN data.
 * @return int 0 on success, -1 if the queue is empty.
 */
static int mock_receive(uint32_t* id, uint8_t* data, uint8_t* len);

/**
 * @brief Mock implementation of CAN send function.
 *
 * @param id CAN ID to send.
 * @param data Pointer to the CAN data to send.
 * @param len Length of the CAN data to send.
 * @return int 0 on success (including when drop is enabled and a frame is discarded), -1 if the
 * queue is full.
 */
static int mock_send(uint32_t id, const uint8_t* data, uint8_t len);

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

static void sleep_ms(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}

static int mock_send(uint32_t id, const uint8_t* data, uint8_t len)
{
    if (drop_enabled)
    {
        return 0;  // frame dropped
    }

    if (disable_fc && len > 0)
    {
        uint8_t pci_type = (uint8_t) ((data[0] >> 4) & 0x0F);
        if (pci_type == 0x3)
        {
            return 0;  // flow control frame dropped
        }
    }

    if ((tail + 1) % MOCK_QUEUE_SIZE == head)
    {
        return -1;  // overflow
    }

    if (delay_ms)
        sleep_ms(delay_ms);

    queue[tail].id = id;
    memcpy(queue[tail].data, data, len);
    queue[tail].len = len;

    tail = (tail + 1) % MOCK_QUEUE_SIZE;

    return 0;
}

static int mock_receive(uint32_t* id, uint8_t* data, uint8_t* len)
{
    if (head == tail)
        return -1;  // empty

    *id = queue[head].id;
    *len = queue[head].len;
    memcpy(data, queue[head].data, *len);

    head = (head + 1) % MOCK_QUEUE_SIZE;
    return 0;
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

void mock_can_init(void)
{
    // clang-format off
    static can_driver_t driver =
    {
        .send = mock_send,
        .receive = mock_receive
    };
    // clang-format on

    head = tail = 0;
    drop_enabled = 0;
    delay_ms = 0;
    disable_fc = 0;

    can_set_driver(&driver);
}

void mock_can_enable_drop(int enable)
{
    drop_enabled = enable;
}

void mock_can_disable_fc(int enable)
{
    disable_fc = enable;
}

void mock_can_set_delay_ms(int delay)
{
    delay_ms = delay;
}