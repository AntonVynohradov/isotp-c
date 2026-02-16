

# File linux\_socket.c

[**File List**](files.md) **>** [**example**](dir_cfafba98a580ce4b62f8a6fa96d7cbb0.md) **>** [**linux\_socket**](dir_c14c1b0e0eb041fc48b83316cb935ab5.md) **>** [**linux\_socket.c**](linux__socket_8c.md)

[Go to the documentation of this file](linux__socket_8c.md)


```C++
/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: Example implementation of ISO-TP over Linux SocketCAN
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <isotp.h>

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define _CAN_INTERFACE "slcan0"  

#define _ISOTP_BUFSIZE \
    (128)  
#define _ISOTP_CAN_ID \
    (0x0C1)  

#define SEC_TO_US(sec) ((sec) * 1000000)  

/* ==============================================================================
 * PRIVATE TYPE DEFINITIONS
 * =============================================================================*/

/* Static type definitions local to this file */

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

static IsoTpLink g_link;  

static uint8_t g_isotpRecvBuf[_ISOTP_BUFSIZE];  
static uint8_t g_isotpSendBuf[_ISOTP_BUFSIZE];  

static int _socket;  

/* ==============================================================================
 * PRIVATE FUNCTION DECLARATIONS (static)
 * =============================================================================*/

uint32_t isotp_user_get_us(void);

void isotp_user_debug(const char* message, ...);

int isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size
#ifdef ISO_TP_USER_SEND_CAN_ARG
                        ,
                        void* arg
#endif
);

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

void isotp_user_debug(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
}

int isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size
#ifdef ISO_TP_USER_SEND_CAN_ARG
                        ,
                        void* arg
#endif
)
{
    int ret = ISOTP_RET_ERROR;

    struct can_frame frame;

    frame.can_id = arbitration_id | CAN_EFF_FLAG;
    frame.can_dlc = size;

    memcpy(frame.data, data, size);

#ifdef ISO_TP_USER_SEND_CAN_ARG
    (void) arg;
#endif
    ssize_t ret_size = write(_socket, &frame, sizeof(struct can_frame));

    if (ret_size == sizeof(struct can_frame))
    {
        ret = ISOTP_RET_OK;
    }

    return ret; /* TODO: Check return value */
}

uint32_t isotp_user_get_us(void)
{
    uint64_t microsecond;
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    {
        perror("Failed to obtain monotonic timestamp.");
        microsecond = UINT64_MAX;  // use this to indicate error
    }
    else
    {
        // `ts` now contains your timestamp in seconds and microseconds! To
        // convert the whole struct to microseconds, do this:
        microsecond = SEC_TO_US((uint64_t) ts.tv_sec) + (uint64_t) (ts.tv_nsec / 1000);
    }

    return (uint32_t) microsecond;
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

int main(int argc, char** argv)
{
    int i;
    int nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    printf("CAN Sockets Demo\r\n");

    _socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (_socket < 0)
    {
        perror("Socket");
        return 1;
    }

    strcpy(ifr.ifr_name, _CAN_INTERFACE);
    ioctl(_socket, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(_socket, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
        perror("Bind");
        return 1;
    }

    /* Init ISOTP lib */
    /* Initialize link, ISOTP_CAN_ID is the CAN ID you send with */
    isotp_init_link(&g_link, _ISOTP_CAN_ID, g_isotpSendBuf, sizeof(g_isotpSendBuf), g_isotpRecvBuf,
                    sizeof(g_isotpRecvBuf));

    while (true)
    {
        nbytes = read(_socket, &frame, sizeof(struct can_frame));
        if (nbytes < 0)
        {
            perror("Read");
            break;
        }

        uint32_t arbitration_id = frame.can_id & CAN_EFF_MASK;
        if (arbitration_id != _ISOTP_CAN_ID)
        {
            continue;
        }

        isotp_on_can_message(&g_link, frame.data, frame.can_dlc);
        isotp_poll(&g_link);

        if (g_link.receive_status == ISOTP_RECEIVE_STATUS_FULL)
        {
            uint8_t payload[_ISOTP_BUFSIZE] = {0};
            uint32_t out_size = 0;
            int ret = isotp_receive(&g_link, payload, sizeof(payload), &out_size);
            if (ret == ISOTP_RET_OK)
            {
                bool is_eff = (frame.can_id & CAN_EFF_FLAG) != 0U;
                uint32_t can_id = frame.can_id & (is_eff ? CAN_EFF_MASK : CAN_SFF_MASK);

                if (is_eff)
                {
                    printf("0x%08X [%d] ", can_id, out_size);
                }
                else
                {
                    printf("0x%03X [%d] ", can_id, out_size);
                }

                for (i = 0; i < out_size; i++)
                    printf("%02X ", payload[i]);

                printf("\r\n");
            }
        }
    }

    isotp_destroy_link(&g_link);

    if (close(_socket) < 0)
    {
        perror("Close");
        return 1;
    }

    return 0;
}
```


