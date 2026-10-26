/**
 * @file main.c
 * @brief Bare-metal example: a UDS-style ECU answering diagnostic requests over ISO-TP.
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
 * The ECU listens on the physical request ID 0x7E0 and the functional ID 0x7DF
 * and answers on 0x7E8. The CAN RX interrupt only copies frames into a ring
 * buffer; the main loop feeds them to the link and polls it. The services are
 * a tiny subset of UDS (ISO 14229) chosen to show both directions of
 * segmentation: the VIN (17 bytes) needs a multi-frame response and writing it
 * needs a multi-frame request.
 */

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <stdbool.h>
#include <string.h>

#include "board.h"
#include "isotp.h"

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define CAN_BITRATE 500000U
#define REQUEST_ID 0x7E0U    /* tester -> ECU, physical */
#define FUNCTIONAL_ID 0x7DFU /* tester -> all ECUs */
#define RESPONSE_ID 0x7E8U   /* ECU -> tester */

#define SID_TESTER_PRESENT 0x3EU
#define SID_READ_DID 0x22U
#define SID_WRITE_DID 0x2EU
#define SID_NEGATIVE 0x7FU
#define POSITIVE(sid) ((uint8_t) ((sid) + 0x40U))
#define NRC_NOT_SUPPORTED 0x11U
#define NRC_LENGTH 0x13U
#define NRC_OUT_OF_RANGE 0x31U
#define SUPPRESS_RESPONSE 0x80U
#define DID_VIN_HIGH 0xF1U
#define DID_VIN_LOW 0x90U
#define VIN_LENGTH 17U

/** Frames buffered between the ISR and the main loop; a power of two. */
#define RX_QUEUE_SIZE 16U

/* ==============================================================================
 * PRIVATE TYPE DEFINITIONS
 * =============================================================================*/

struct can_frame
{
    uint32_t id;
    uint8_t length;
    uint8_t data[8];
};

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

static struct isotp_link link;
static uint8_t vin[VIN_LENGTH] = {'S', 'F', 'S', 'I', 'S', 'O', 'T', 'P', 'C',
                                  '2', '0', '0', '0', '0', '0', '0', '1'};

/*
 * The ISR writes rx_head, the main loop writes rx_tail: safe without locking on
 * a single-core MCU with atomic 8-bit stores.
 */
static volatile struct can_frame rx_queue[RX_QUEUE_SIZE];
static volatile uint8_t rx_head;
static volatile uint8_t rx_tail;
static volatile uint32_t rx_overruns;

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

static int can_write(void* context, uint32_t can_id, const uint8_t* data, size_t length)
{
    (void) context;
    return board_can_write(can_id, data, length);
}

static void respond(const uint8_t* data, uint32_t length)
{
    /* A tester waits for the answer before its next request, so the link is idle */
    (void) isotp_link_send(&link, data, length, board_micros());
}

static void negative(uint8_t sid, uint8_t nrc)
{
    const uint8_t answer[3] = {SID_NEGATIVE, sid, nrc};
    respond(answer, sizeof(answer));
}

static void read_did(const uint8_t* request, uint32_t length)
{
    uint8_t answer[3U + VIN_LENGTH];

    if (length != 3U)
    {
        negative(SID_READ_DID, NRC_LENGTH);
    }
    else if ((request[1] != DID_VIN_HIGH) || (request[2] != DID_VIN_LOW))
    {
        negative(SID_READ_DID, NRC_OUT_OF_RANGE);
    }
    else
    {
        answer[0] = POSITIVE(SID_READ_DID);
        answer[1] = DID_VIN_HIGH;
        answer[2] = DID_VIN_LOW;
        (void) memcpy(&answer[3], vin, VIN_LENGTH);
        respond(answer, sizeof(answer));
    }
}

static void write_did(const uint8_t* request, uint32_t length)
{
    if (length != (3U + VIN_LENGTH))
    {
        negative(SID_WRITE_DID, NRC_LENGTH);
    }
    else if ((request[1] != DID_VIN_HIGH) || (request[2] != DID_VIN_LOW))
    {
        negative(SID_WRITE_DID, NRC_OUT_OF_RANGE);
    }
    else
    {
        const uint8_t answer[3] = {POSITIVE(SID_WRITE_DID), DID_VIN_HIGH, DID_VIN_LOW};
        (void) memcpy(vin, &request[3], VIN_LENGTH);
        respond(answer, sizeof(answer));
    }
}

/* Message handler: runs in the main loop, inside isotp_link_receive(). */
static void on_request(const uint8_t* data, uint32_t length, void* context)
{
    (void) context;
    switch (data[0])
    {
        case SID_TESTER_PRESENT:
            if ((length == 2U) && ((data[1] & SUPPRESS_RESPONSE) == 0U))
            {
                const uint8_t answer[2] = {POSITIVE(SID_TESTER_PRESENT), 0x00U};
                respond(answer, sizeof(answer));
            }
            break;
        case SID_READ_DID:
            read_did(data, length);
            break;
        case SID_WRITE_DID:
            write_did(data, length);
            break;
        default:
            negative(data[0], NRC_NOT_SUPPORTED);
            break;
    }
}

static bool rx_pop(struct can_frame* frame)
{
    bool available = (rx_tail != rx_head);

    if (available)
    {
        const uint8_t tail = rx_tail;
        frame->id = rx_queue[tail].id;
        frame->length = rx_queue[tail].length;
        for (uint8_t i = 0U; i < frame->length; i++)
        {
            frame->data[i] = rx_queue[tail].data[i];
        }
        rx_tail = (uint8_t) ((tail + 1U) & (RX_QUEUE_SIZE - 1U));
    }
    return available;
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

void app_can_rx_isr(uint32_t can_id, const uint8_t* data, uint8_t length)
{
    const uint8_t head = rx_head;
    const uint8_t next = (uint8_t) ((head + 1U) & (RX_QUEUE_SIZE - 1U));

    if ((next == rx_tail) || (length > 8U))
    {
        rx_overruns++;
    }
    else
    {
        rx_queue[head].id = can_id;
        rx_queue[head].length = length;
        for (uint8_t i = 0U; i < length; i++)
        {
            rx_queue[head].data[i] = data[i];
        }
        rx_head = next;
    }
}

int main(void)
{
    struct isotp_link_config config;
    struct can_frame frame;

    board_init(CAN_BITRATE);

    isotp_link_config_default(&config);
    config.tx_id = RESPONSE_ID;
    config.write = can_write;
    config.on_message = on_request;
    config.block_size = 2U;   /* ask the tester for Flow Control every 2 frames */
    config.st_min_us = 1000U; /* and 1 ms between its Consecutive Frames */
    if (isotp_link_init(&link, &config) != ISOTP_OK)
    {
        return 1;
    }

    for (;;)
    {
        while (rx_pop(&frame))
        {
            if ((frame.id == REQUEST_ID) || (frame.id == FUNCTIONAL_ID))
            {
                isotp_link_receive(&link, frame.data, frame.length, board_micros());
            }
        }
        isotp_link_poll(&link, board_micros());
        board_idle();
    }
}
