/**
 * @file main.c
 * @brief Zephyr example: the UDS-style ECU on the Zephyr CAN API (chosen node zephyr,canbus).
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
 * The CAN driver puts frames that pass the filters into a k_msgq from its
 * interrupt; one thread takes them out, feeds the link and polls it every
 * millisecond. Only that thread touches the link, so no mutex is needed; an
 * application that sends from another thread wraps isotp_link_send() and the
 * calls of this thread in one k_mutex. Not built in CI (needs a Zephyr
 * workspace): west build -b <board> examples/zephyr
 */

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <string.h>

#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "isotp.h"

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define REQUEST_ID 0x7E0U
#define FUNCTIONAL_ID 0x7DFU
#define RESPONSE_ID 0x7E8U

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

static const struct device* const can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));
static struct isotp_link link;

CAN_MSGQ_DEFINE(rx_msgq, 16);

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

static uint32_t now_us(void)
{
    return k_cyc_to_us_floor32(k_cycle_get_32());
}

static int can_write(void* context, uint32_t can_id, const uint8_t* data, size_t length)
{
    struct can_frame frame = {0};
    int result;

    (void) context;
    frame.id = can_id;
    frame.dlc = can_bytes_to_dlc((uint8_t) length);
    memcpy(frame.data, data, length);
    result = can_send(can_dev, &frame, K_NO_WAIT, NULL, NULL);
    return (result == -EAGAIN) ? ISOTP_BUSY : result;
}

static void on_request(const uint8_t* data, uint32_t length, void* context)
{
    (void) context;
    if ((length == 2U) && (data[0] == 0x3EU) && ((data[1] & 0x80U) == 0U))
    {
        static const uint8_t answer[] = {0x7EU, 0x00U}; /* TesterPresent */
        (void) isotp_link_send(&link, answer, sizeof(answer), now_us());
    }
    else
    {
        const uint8_t answer[] = {0x7FU, data[0], 0x11U}; /* serviceNotSupported */
        (void) isotp_link_send(&link, answer, sizeof(answer), now_us());
    }
}

static int add_filter(uint32_t id)
{
    const struct can_filter filter = {.id = id, .mask = CAN_STD_ID_MASK, .flags = 0U};

    return can_add_rx_filter_msgq(can_dev, &rx_msgq, &filter);
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

int main(void)
{
    struct isotp_link_config config;
    struct can_frame frame;

    if (!device_is_ready(can_dev) || (can_start(can_dev) != 0) || (add_filter(REQUEST_ID) < 0)
        || (add_filter(FUNCTIONAL_ID) < 0))
    {
        printk("CAN device not ready\n");
        return 0;
    }

    isotp_link_config_default(&config);
    config.tx_id = RESPONSE_ID;
    config.write = can_write;
    config.on_message = on_request;
    config.padding = true;
    (void) isotp_link_init(&link, &config);
    printk("ISOTP-C %s: ECU on 0x%03X/0x%03X\n", ISOTP_VERSION, REQUEST_ID, RESPONSE_ID);

    for (;;)
    {
        if (k_msgq_get(&rx_msgq, &frame, K_MSEC(1)) == 0)
        {
            isotp_link_receive(&link, frame.data, can_dlc_to_bytes(frame.dlc), now_us());
        }
        isotp_link_poll(&link, now_us());
    }
    return 0;
}
