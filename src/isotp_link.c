/**
 * @file isotp_link.c
 * @brief ISO-TP link: transmitter and receiver state machines (ISO 15765-2).
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

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <string.h>

#include "isotp_link.h"

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define ISOTP_DEFAULT_PADDING_VALUE 0xCCU
#define ISOTP_TIMEOUT_MAX_US 0x7FFFFFFFU /* deadlines are compared modulo 2^32 */
#define ISOTP_TIME_HALF_RANGE 0x80000000U

/* ==============================================================================
 * PRIVATE FUNCTION DECLARATIONS (static)
 * =============================================================================*/

static bool isotp_time_reached(uint32_t now_us, uint32_t deadline_us);
static uint32_t isotp_min_u32(uint32_t a, uint32_t b);
static int isotp_link_write(const struct isotp_link* link, uint32_t can_id, uint8_t* frame,
                            int length);
static int isotp_link_send_fc(const struct isotp_link* link, uint8_t flow_status);
static void isotp_link_tx_finish(struct isotp_link* link, int n_result);
static void isotp_link_rx_fail(struct isotp_link* link, int n_result);
static void isotp_link_rx_interrupt(struct isotp_link* link);
static void isotp_link_rx_single(struct isotp_link* link, const uint8_t* data,
                                 const struct isotp_pdu* pdu);
static void isotp_link_rx_first(struct isotp_link* link, const uint8_t* data,
                                const struct isotp_pdu* pdu, uint32_t now_us);
static void isotp_link_rx_consecutive(struct isotp_link* link, const uint8_t* data,
                                      const struct isotp_pdu* pdu, uint32_t now_us);
static void isotp_link_rx_flow_control(struct isotp_link* link, const struct isotp_pdu* pdu,
                                       uint32_t now_us);
static void isotp_link_tx_consecutive(struct isotp_link* link, uint32_t now_us);

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

/* True when now_us is at or after deadline_us; valid for distances below 2^31 us. */
static bool isotp_time_reached(uint32_t now_us, uint32_t deadline_us)
{
    return ((uint32_t) (now_us - deadline_us)) < ISOTP_TIME_HALF_RANGE;
}

static uint32_t isotp_min_u32(uint32_t a, uint32_t b)
{
    return (a < b) ? a : b;
}

/*
 * Pad (when configured) and write one encoded frame. The callers pass only
 * lengths returned by a successful encoder call: their arguments are checked
 * before, so the encoders cannot fail here and length is 1..8.
 */
static int isotp_link_write(const struct isotp_link* link, uint32_t can_id, uint8_t* frame,
                            int length)
{
    size_t frame_length = (size_t) length;

    if (link->config.padding)
    {
        frame_length = (size_t) isotp_frame_pad(frame, (size_t) ISOTP_CAN_DL, frame_length,
                                                link->config.padding_value);
    }
    return link->config.write(link->config.write_context, can_id, frame, frame_length);
}

static int isotp_link_send_fc(const struct isotp_link* link, uint8_t flow_status)
{
    uint8_t frame[ISOTP_CAN_DL];
    int length =
        isotp_encode_flow_control(frame, sizeof(frame), flow_status, link->config.block_size,
                                  isotp_st_min_encode(link->config.st_min_us));

    return isotp_link_write(link, link->config.tx_id, frame, length);
}

static void isotp_link_tx_finish(struct isotp_link* link, int n_result)
{
    link->tx_state = ISOTP_TX_IDLE;
    link->tx_result = n_result;
    if (link->config.on_tx_done != NULL)
    {
        link->config.on_tx_done(n_result, link->config.app_context);
    }
}

static void isotp_link_rx_fail(struct isotp_link* link, int n_result)
{
    link->rx_state = ISOTP_RX_IDLE;
    link->rx_result = n_result;
    if (link->config.on_rx_error != NULL)
    {
        link->config.on_rx_error(n_result, link->config.app_context);
    }
}

/* A new SF or FF ends a reception in progress (N_UNEXP_PDU) and is processed. */
static void isotp_link_rx_interrupt(struct isotp_link* link)
{
    if (link->rx_state == ISOTP_RX_RECEIVING)
    {
        isotp_link_rx_fail(link, ISOTP_N_UNEXP_PDU);
    }
}

static void isotp_link_rx_single(struct isotp_link* link, const uint8_t* data,
                                 const struct isotp_pdu* pdu)
{
    isotp_link_rx_interrupt(link);
    link->rx_result = ISOTP_N_OK;
    if (link->config.on_message != NULL)
    {
        link->config.on_message(&data[pdu->data_offset], pdu->message_length,
                                link->config.app_context);
    }
}

static void isotp_link_rx_first(struct isotp_link* link, const uint8_t* data,
                                const struct isotp_pdu* pdu, uint32_t now_us)
{
    isotp_link_rx_interrupt(link);

    if (pdu->message_length > ISOTP_RX_BUFFER_SIZE)
    {
        /* The sender aborts on OVFLW; nothing to retry if this write fails */
        (void) isotp_link_send_fc(link, ISOTP_FS_OVERFLOW);
        isotp_link_rx_fail(link, ISOTP_N_BUFFER_OVFLW);
    }
    else
    {
        /* data_length is 6 or 2 and message_length > 7, so the copy fits the buffer */
        (void) memcpy(link->rx_buffer, &data[pdu->data_offset], (size_t) pdu->data_length);
        link->rx_size = pdu->message_length;
        link->rx_offset = pdu->data_length;
        link->rx_sn = ISOTP_FIRST_SN;
        link->rx_block_count = link->config.block_size;

        if (isotp_link_send_fc(link, ISOTP_FS_CONTINUE) == ISOTP_OK)
        {
            link->rx_state = ISOTP_RX_RECEIVING;
            link->rx_deadline_us = now_us + link->config.n_cr_timeout_us;
        }
        else
        {
            isotp_link_rx_fail(link, ISOTP_N_ERROR);
        }
    }
}

static void isotp_link_rx_consecutive(struct isotp_link* link, const uint8_t* data,
                                      const struct isotp_pdu* pdu, uint32_t now_us)
{
    if (link->rx_state != ISOTP_RX_RECEIVING)
    {
        /* Not expecting a Consecutive Frame: ISO 15765-2 says ignore it */
    }
    else if (pdu->sequence_number != link->rx_sn)
    {
        isotp_link_rx_fail(link, ISOTP_N_WRONG_SN);
    }
    else
    {
        uint32_t take = isotp_min_u32(link->rx_size - link->rx_offset, ISOTP_CF_MAX_DATA);

        if ((uint32_t) pdu->data_length < take)
        {
            /* Too short for the bytes still expected */
            link->invalid_frames++;
        }
        else
        {
            /* rx_offset + take <= rx_size <= ISOTP_RX_BUFFER_SIZE */
            (void) memcpy(&link->rx_buffer[link->rx_offset], &data[pdu->data_offset],
                          (size_t) take);
            link->rx_offset += take;
            link->rx_sn = (uint8_t) ((link->rx_sn + 1U) & ISOTP_SN_MASK);
            link->rx_deadline_us = now_us + link->config.n_cr_timeout_us;

            if (link->rx_offset == link->rx_size)
            {
                link->rx_state = ISOTP_RX_IDLE;
                link->rx_result = ISOTP_N_OK;
                if (link->config.on_message != NULL)
                {
                    link->config.on_message(link->rx_buffer, link->rx_size,
                                            link->config.app_context);
                }
            }
            else if (link->config.block_size != 0U)
            {
                link->rx_block_count--;
                if (link->rx_block_count == 0U)
                {
                    link->rx_block_count = link->config.block_size;
                    if (isotp_link_send_fc(link, ISOTP_FS_CONTINUE) != ISOTP_OK)
                    {
                        isotp_link_rx_fail(link, ISOTP_N_ERROR);
                    }
                }
            }
            else
            {
                /* BS = 0: the sender continues without further Flow Control */
            }
        }
    }
}

static void isotp_link_rx_flow_control(struct isotp_link* link, const struct isotp_pdu* pdu,
                                       uint32_t now_us)
{
    if (link->tx_state != ISOTP_TX_WAIT_FC)
    {
        /* Not waiting for Flow Control: ISO 15765-2 says ignore it */
    }
    else if (pdu->flow_status == ISOTP_FS_CONTINUE)
    {
        link->tx_block_unlimited = (pdu->block_size == 0U);
        link->tx_block_remaining = pdu->block_size;
        link->tx_st_min_us = isotp_st_min_decode(pdu->st_min);
        link->tx_wft_count = 0U;
        link->tx_state = ISOTP_TX_SEND_CF;
        link->tx_deadline_us = now_us; /* the first CF of a block may follow at once */
    }
    else if (pdu->flow_status == ISOTP_FS_WAIT)
    {
        if (link->tx_wft_count < link->config.wft_max)
        {
            link->tx_wft_count++;
            link->tx_deadline_us = now_us + link->config.n_bs_timeout_us;
        }
        else
        {
            isotp_link_tx_finish(link, ISOTP_N_WFT_OVRN);
        }
    }
    else if (pdu->flow_status == ISOTP_FS_OVERFLOW)
    {
        isotp_link_tx_finish(link, ISOTP_N_BUFFER_OVFLW);
    }
    else
    {
        isotp_link_tx_finish(link, ISOTP_N_INVALID_FS);
    }
}

static void isotp_link_tx_consecutive(struct isotp_link* link, uint32_t now_us)
{
    uint8_t frame[ISOTP_CAN_DL];
    uint32_t take = isotp_min_u32(link->tx_size - link->tx_offset, ISOTP_CF_MAX_DATA);
    int length = isotp_encode_consecutive(frame, sizeof(frame), link->tx_sn,
                                          &link->tx_buffer[link->tx_offset], take);
    int written = isotp_link_write(link, link->config.tx_id, frame, length);

    if (written == ISOTP_OK)
    {
        link->tx_offset += take;
        link->tx_sn = (uint8_t) ((link->tx_sn + 1U) & ISOTP_SN_MASK);
        link->tx_deadline_us = now_us + link->tx_st_min_us;

        if (link->tx_offset == link->tx_size)
        {
            isotp_link_tx_finish(link, ISOTP_N_OK);
        }
        else if (!link->tx_block_unlimited)
        {
            /* tx_block_remaining >= 1 here: BS > 0 and the block is not finished */
            link->tx_block_remaining--;
            if (link->tx_block_remaining == 0U)
            {
                link->tx_state = ISOTP_TX_WAIT_FC;
                link->tx_deadline_us = now_us + link->config.n_bs_timeout_us;
            }
        }
        else
        {
            /* BS = 0: keep sending */
        }
    }
    else if (written == ISOTP_BUSY)
    {
        /* Driver queue full: retry on the next poll */
    }
    else
    {
        isotp_link_tx_finish(link, ISOTP_N_ERROR);
    }
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

void isotp_link_config_default(struct isotp_link_config* config)
{
    if (config != NULL)
    {
        config->tx_id = 0U;
        config->write = NULL;
        config->write_context = NULL;
        config->on_message = NULL;
        config->on_tx_done = NULL;
        config->on_rx_error = NULL;
        config->app_context = NULL;
        config->block_size = (uint8_t) ISOTP_DEFAULT_BLOCK_SIZE;
        config->st_min_us = ISOTP_DEFAULT_ST_MIN_US;
        config->n_bs_timeout_us = ISOTP_DEFAULT_N_BS_TIMEOUT_US;
        config->n_cr_timeout_us = ISOTP_DEFAULT_N_CR_TIMEOUT_US;
        config->wft_max = (uint8_t) ISOTP_DEFAULT_WFT_MAX;
        config->padding = false;
        config->padding_value = ISOTP_DEFAULT_PADDING_VALUE;
    }
}

int isotp_link_init(struct isotp_link* link, const struct isotp_link_config* config)
{
    int result = ISOTP_ERROR;

    if ((link != NULL) && (config != NULL) && (config->write != NULL)
        && (config->st_min_us <= ISOTP_ST_MIN_MAX_US) && (config->n_bs_timeout_us > 0U)
        && (config->n_bs_timeout_us <= ISOTP_TIMEOUT_MAX_US) && (config->n_cr_timeout_us > 0U)
        && (config->n_cr_timeout_us <= ISOTP_TIMEOUT_MAX_US))
    {
        (void) memset(link, 0, sizeof(*link));
        link->config = *config;
        link->tx_state = ISOTP_TX_IDLE;
        link->rx_state = ISOTP_RX_IDLE;
        link->tx_result = ISOTP_N_OK;
        link->rx_result = ISOTP_N_OK;
        result = ISOTP_OK;
    }
    return result;
}

void isotp_link_receive(struct isotp_link* link, const uint8_t* data, size_t length,
                        uint32_t now_us)
{
    struct isotp_pdu pdu;

    if (link != NULL)
    {
        if (isotp_decode(data, length, &pdu) != ISOTP_OK)
        {
            link->invalid_frames++;
        }
        else if (pdu.type == ISOTP_PCI_SINGLE_FRAME)
        {
            isotp_link_rx_single(link, data, &pdu);
        }
        else if (pdu.type == ISOTP_PCI_FIRST_FRAME)
        {
            isotp_link_rx_first(link, data, &pdu, now_us);
        }
        else if (pdu.type == ISOTP_PCI_CONSECUTIVE_FRAME)
        {
            isotp_link_rx_consecutive(link, data, &pdu, now_us);
        }
        else
        {
            /* isotp_decode accepts only the four frame types: this is Flow Control */
            isotp_link_rx_flow_control(link, &pdu, now_us);
        }
    }
}

int isotp_link_send(struct isotp_link* link, const uint8_t* payload, uint32_t length,
                    uint32_t now_us)
{
    int result = ISOTP_ERROR;
    uint8_t frame[ISOTP_CAN_DL];

    if ((link == NULL) || (payload == NULL) || (length == 0U) || (length > ISOTP_TX_BUFFER_SIZE))
    {
        /* result stays ISOTP_ERROR */
    }
    else if (link->tx_state != ISOTP_TX_IDLE)
    {
        result = ISOTP_BUSY;
    }
    else if (length <= ISOTP_SF_MAX_DL)
    {
        int frame_length = isotp_encode_single(frame, sizeof(frame), payload, length);

        result = isotp_link_write(link, link->config.tx_id, frame, frame_length);
        if (result == ISOTP_OK)
        {
            isotp_link_tx_finish(link, ISOTP_N_OK);
        }
    }
    else
    {
        int frame_length;

        (void) memcpy(link->tx_buffer, payload, (size_t) length);
        frame_length = isotp_encode_first(frame, sizeof(frame), link->tx_buffer, length);
        result = isotp_link_write(link, link->config.tx_id, frame, frame_length);
        if (result == ISOTP_OK)
        {
            link->tx_size = length;
            link->tx_offset = (length <= ISOTP_FF_DL_12BIT_MAX) ? (uint32_t) ISOTP_FF_SHORT_DATA :
                                                                  (uint32_t) ISOTP_FF_LONG_DATA;
            link->tx_sn = ISOTP_FIRST_SN;
            link->tx_wft_count = 0U;
            link->tx_state = ISOTP_TX_WAIT_FC;
            link->tx_deadline_us = now_us + link->config.n_bs_timeout_us;
        }
    }
    return result;
}

int isotp_link_send_functional(const struct isotp_link* link, uint32_t can_id,
                               const uint8_t* payload, uint32_t length)
{
    int result = ISOTP_ERROR;

    if ((link != NULL) && (payload != NULL) && (length > 0U) && (length <= ISOTP_SF_MAX_DL))
    {
        uint8_t frame[ISOTP_CAN_DL];
        int frame_length = isotp_encode_single(frame, sizeof(frame), payload, length);

        result = isotp_link_write(link, can_id, frame, frame_length);
    }
    return result;
}

void isotp_link_poll(struct isotp_link* link, uint32_t now_us)
{
    if (link != NULL)
    {
        if (link->tx_state == ISOTP_TX_IDLE)
        {
            /* nothing to send */
        }
        else if (!isotp_time_reached(now_us, link->tx_deadline_us))
        {
            /* waiting for Flow Control or for STmin */
        }
        else if (link->tx_state == ISOTP_TX_WAIT_FC)
        {
            isotp_link_tx_finish(link, ISOTP_N_TIMEOUT_BS);
        }
        else
        {
            isotp_link_tx_consecutive(link, now_us);
        }

        if ((link->rx_state == ISOTP_RX_RECEIVING)
            && isotp_time_reached(now_us, link->rx_deadline_us))
        {
            isotp_link_rx_fail(link, ISOTP_N_TIMEOUT_CR);
        }
    }
}

bool isotp_link_tx_busy(const struct isotp_link* link)
{
    return (link != NULL) && (link->tx_state != ISOTP_TX_IDLE);
}

int isotp_link_tx_result(const struct isotp_link* link)
{
    return (link != NULL) ? link->tx_result : ISOTP_N_ERROR;
}

int isotp_link_rx_result(const struct isotp_link* link)
{
    return (link != NULL) ? link->rx_result : ISOTP_N_ERROR;
}

uint32_t isotp_link_invalid_frames(const struct isotp_link* link)
{
    return (link != NULL) ? link->invalid_frames : 0U;
}
