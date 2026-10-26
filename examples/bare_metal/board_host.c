/**
 * @file board_host.c
 * @brief Simulated board for the bare-metal example: a tester on a virtual CAN bus (CTest).
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
 * Runs the unmodified main.c on a PC. board_idle() plays the tester: it injects
 * line noise (a malformed frame on the request ID and a frame of another node),
 * sends the next diagnostic request through its own ISO-TP link, moves frames
 * between the two nodes, advances a simulated clock by 100 us and checks every
 * response. The process exits with 0 when every response matches.
 */

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "isotp.h"

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define REQUEST_ID 0x7E0U
#define FUNCTIONAL_ID 0x7DFU
#define RESPONSE_ID 0x7E8U
#define TICK_US 100U
#define TIME_LIMIT_US 10000000U /* 10 s of simulated time */
#define MAX_PENDING 64U

/* ==============================================================================
 * PRIVATE TYPE DEFINITIONS
 * =============================================================================*/

struct exchange
{
    const char* name;
    bool functional;
    uint8_t request[24];
    uint32_t request_length;
    uint8_t response[24];
    uint32_t response_length; /* 0: no response expected */
};

struct pending_frame
{
    uint32_t id;
    uint8_t length;
    uint8_t data[8];
};

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

static const struct exchange exchanges[] = {
    {"TesterPresent (functional)", true, {0x3E, 0x00}, 2U, {0x7E, 0x00}, 2U},
    {"ReadDataByIdentifier VIN",
     false,
     {0x22, 0xF1, 0x90},
     3U,
     {0x62, 0xF1, 0x90, 'S', 'F', 'S', 'I', 'S', 'O', 'T',
      'P',  'C',  '2',  '0', '0', '0', '0', '0', '0', '1'},
     20U},
    {"WriteDataByIdentifier VIN",
     false,
     {0x2E, 0xF1, 0x90, 'S', 'F', 'S', 'I', 'S', 'O', 'T',
      'P',  'C',  '2',  '0', '0', '0', '0', '0', '0', '2'},
     20U,
     {0x6E, 0xF1, 0x90},
     3U},
    {"ReadDataByIdentifier VIN again",
     false,
     {0x22, 0xF1, 0x90},
     3U,
     {0x62, 0xF1, 0x90, 'S', 'F', 'S', 'I', 'S', 'O', 'T',
      'P',  'C',  '2',  '0', '0', '0', '0', '0', '0', '2'},
     20U},
    {"Unsupported service", false, {0x10, 0x01}, 2U, {0x7F, 0x10, 0x11}, 3U},
    {"TesterPresent, response suppressed", false, {0x3E, 0x80}, 2U, {0}, 0U},
};

static struct isotp_link tester;
static uint32_t now_us;
static size_t next_exchange;
static bool waiting;
static uint32_t waiting_since;
static int failures;
static struct pending_frame to_tester[MAX_PENDING];
static size_t to_tester_count;

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

static void print_hex(const char* label, const uint8_t* data, uint32_t length)
{
    printf("  %s", label);
    for (uint32_t i = 0U; i < length; i++)
    {
        printf(" %02X", data[i]);
    }
    printf("\n");
}

/* Tester -> ECU: straight into the ECU's RX interrupt. */
static int tester_write(void* context, uint32_t can_id, const uint8_t* data, size_t length)
{
    (void) context;
    app_can_rx_isr(can_id, data, (uint8_t) length);
    return 0;
}

static void finish_exchange(bool ok)
{
    printf("%-36s %s\n", exchanges[next_exchange].name, ok ? "OK" : "FAILED");
    if (!ok)
    {
        failures++;
    }
    waiting = false;
    next_exchange++;
}

static void on_response(const uint8_t* data, uint32_t length, void* context)
{
    const struct exchange* exchange = &exchanges[next_exchange];

    (void) context;
    print_hex("response:", data, length);
    finish_exchange(waiting && (length == exchange->response_length)
                    && (memcmp(data, exchange->response, length) == 0));
}

static void start_exchange(void)
{
    static const uint8_t malformed[] = {0x00U, 0x55U};         /* SF_DL = 0 */
    static const uint8_t other_node[] = {0x02U, 0x01U, 0x0DU}; /* another ECU's traffic */
    const struct exchange* exchange = &exchanges[next_exchange];
    int result;

    app_can_rx_isr(REQUEST_ID, malformed, sizeof(malformed));
    app_can_rx_isr(0x123U, other_node, sizeof(other_node));

    print_hex("request: ", exchange->request, exchange->request_length);
    if (exchange->functional)
    {
        result = isotp_link_send_functional(&tester, FUNCTIONAL_ID, exchange->request,
                                            exchange->request_length);
    }
    else
    {
        result = isotp_link_send(&tester, exchange->request, exchange->request_length, now_us);
    }
    if (result != ISOTP_OK)
    {
        printf("send failed: %d\n", result);
        exit(EXIT_FAILURE);
    }
    waiting = true;
    waiting_since = now_us;
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS (board.h)
 * =============================================================================*/

void board_init(uint32_t bitrate)
{
    struct isotp_link_config config;

    printf("Simulated CAN bus at %u bit/s: ECU 0x%03X/0x%03X, tester with BS 4, STmin 2 ms\n",
           (unsigned) bitrate, REQUEST_ID, RESPONSE_ID);
    isotp_link_config_default(&config);
    config.tx_id = REQUEST_ID;
    config.write = tester_write;
    config.on_message = on_response;
    config.block_size = 4U;
    config.st_min_us = 2000U;
    if (isotp_link_init(&tester, &config) != ISOTP_OK)
    {
        exit(EXIT_FAILURE);
    }
}

/* ECU -> tester: queued and delivered by board_idle(), like a real bus. */
int board_can_write(uint32_t can_id, const uint8_t* data, size_t length)
{
    int result = ISOTP_BUSY;

    if (to_tester_count < MAX_PENDING)
    {
        to_tester[to_tester_count].id = can_id;
        to_tester[to_tester_count].length = (uint8_t) length;
        (void) memcpy(to_tester[to_tester_count].data, data, length);
        to_tester_count++;
        result = 0;
    }
    return result;
}

uint32_t board_micros(void)
{
    return now_us;
}

void board_idle(void)
{
    now_us += TICK_US;

    for (size_t i = 0U; i < to_tester_count; i++)
    {
        if (to_tester[i].id == RESPONSE_ID)
        {
            isotp_link_receive(&tester, to_tester[i].data, to_tester[i].length, now_us);
        }
    }
    to_tester_count = 0U;
    isotp_link_poll(&tester, now_us);

    if (waiting && (exchanges[next_exchange].response_length == 0U)
        && ((now_us - waiting_since) > 50000U))
    {
        finish_exchange(true); /* no response within 50 ms, as expected */
    }

    if (!waiting)
    {
        if (next_exchange < (sizeof(exchanges) / sizeof(exchanges[0])))
        {
            start_exchange();
        }
        else
        {
            printf("%d of %u exchanges failed\n", failures,
                   (unsigned) (sizeof(exchanges) / sizeof(exchanges[0])));
            exit((failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
        }
    }

    if (now_us > TIME_LIMIT_US)
    {
        printf("timeout: no response to \"%s\"\n", exchanges[next_exchange].name);
        exit(EXIT_FAILURE);
    }
}
