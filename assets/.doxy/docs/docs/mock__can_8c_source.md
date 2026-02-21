

# File mock\_can.c

[**File List**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**python\_api**](dir_181ea403426f4b233011860116dcb1b5.md) **>** [**src**](dir_63ad0292e4d57c8e6c47fd30f33819f0.md) **>** [**mock\_can.c**](mock__can_8c.md)

[Go to the documentation of this file](mock__can_8c.md)


```C++
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

#include "mock_can.h"
#include "can_driver.h"
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define MOCK_QUEUE_SIZE 256  

#define CAN_MAX_DLEN 8  

/* ==============================================================================
 * PRIVATE TYPE DEFINITIONS
 * =============================================================================*/

typedef struct
{
    uint32_t id;
    uint8_t data[CAN_MAX_DLEN];
    uint8_t len;

} can_frame_t;

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

static can_frame_t queue[MOCK_QUEUE_SIZE];  

static int head = 0;  
static int tail = 0;  

static int drop_enabled = 0;  
static int delay_ms = 0;  
static int disable_fc = 0;  

/* ==============================================================================
 * PRIVATE FUNCTION DECLARATIONS (static)
 * =============================================================================*/

static void sleep_ms(int ms);

static int mock_receive(uint32_t* id, uint8_t* data, uint8_t* len);

static int mock_send(uint32_t id, const uint8_t* data, uint8_t len);

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

static void sleep_ms(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    if (ms > 0)
    {
        usleep((unsigned int) ms * 1000);
    }
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
    {
        sleep_ms(delay_ms);
    }

    queue[tail].id = id;
    memcpy(queue[tail].data, data, len);
    queue[tail].len = len;

    tail = (tail + 1) % MOCK_QUEUE_SIZE;

    return 0;
}

static int mock_receive(uint32_t* id, uint8_t* data, uint8_t* len)
{
    if (head == tail)
    {
        return -1;  // empty
    }

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
```


