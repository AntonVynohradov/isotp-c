/**
 * @file isotp.h
 * @brief Core ISO-TP protocol API with link management and message handling.
 *
 * Project: ISO-TP-C - Embedded-Grade Refactoring & Optimization
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

/**
 * @addtogroup isotp ISO-TP API
 * @brief Public API for ISO-TP link management and message handling.
 *
 * @details This group defines the public interface for initializing links, sending
 * and receiving messages, handling incoming CAN frames, and polling the
 * protocol state machine. It also exposes optional callback registration
 * when enabled in configuration.
 *
 * @par Public interface includes:
 * @li Link lifecycle: @ref isotp_init_link, @ref isotp_destroy_link.
 * @li Message flow: @ref isotp_send, @ref isotp_send_with_id,
 *     @ref isotp_on_can_message, @ref isotp_receive, @ref isotp_poll.
 * @li Link data model: @ref IsoTpLink.
 * @li Optional callbacks: @ref isotp_set_tx_done_cb, @ref isotp_set_rx_done_cb.
 * @{
 */

#ifndef ISOTPC_H
#define ISOTPC_H

/** @name Includes */
/** @{ */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "isotp_config.h"
#include "isotp_defines.h"
#include "isotp_user.h"

/** @} */


/** @name Type definitions */
/** @{ */

/**
 * @brief Link state for a single ISO-TP connection.
 * @details The data stored in this struct is used internally and may be used by
 *          software programs using this library.
 */
typedef struct IsoTpLink
{
    /* sender parameters */
    uint32_t send_arbitration_id; /**< Arbitration ID used for consecutive frames. */

    /* message buffer */
    uint8_t* send_buffer;   /**< Send buffer base pointer. */
    uint32_t send_buf_size; /**< Send buffer size in bytes. */
    uint32_t send_size;     /**< Total size to send in bytes. */
    uint32_t send_offset;   /**< Current send offset in bytes. */

    /* multi-frame flags */
    uint8_t send_sn;              /**< Current sequence number. */
    uint32_t send_bs_remain;      /**< Remaining block size. */
    uint32_t send_st_min_us;      /**< Minimum separation time in microseconds. */
    uint8_t send_wtf_count;       /**< Max number of FC.Wait frames to send. */
    uint32_t send_timer_st;       /**< Timestamp of last consecutive frame send. */
    uint32_t send_timer_bs;       /**< Timer until next FlowControl is expected. */
    int32_t send_protocol_result; /**< Last protocol result code for sender. */
    uint8_t send_status;          /**< Sender status. */

    /* receiver parameters */
    uint32_t receive_arbitration_id; /**< Arbitration ID used for reception. */

    /* message buffer */
    uint8_t* receive_buffer;   /**< Receive buffer base pointer. */
    uint32_t receive_buf_size; /**< Receive buffer size in bytes. */
    uint32_t receive_size;     /**< Total received size in bytes. */
    uint32_t receive_offset;   /**< Current receive offset in bytes. */

    /* multi-frame control */
    uint8_t receive_sn;          /**< Expected sequence number. */
    uint8_t receive_bs_count;    /**< Max number of FC.Wait frames to send. */
    uint32_t receive_timer_cr;   /**< Timer until next ConsecutiveFrame is expected. */
    int receive_protocol_result; /**< Last protocol result code for receiver. */
    uint8_t receive_status;      /**< Receiver status. */

#if defined(ISO_TP_USER_SEND_CAN_ARG)
    void* user_send_can_arg; /**< User argument for `isotp_user_send_can`. */
#endif

#ifdef ISO_TP_TRANSMIT_COMPLETE_CALLBACK
    isotp_tx_done_cb tx_done_cb; /**< User callback for transmission complete. */
    void* tx_done_cb_arg;        /**< User argument for callback. */
#endif

#ifdef ISO_TP_RECEIVE_COMPLETE_CALLBACK
    isotp_rx_done_cb rx_done_cb; /**< User callback for receive complete. */
    void* rx_done_cb_arg;        /**< User argument for callback. */
#endif

} IsoTpLink;

/** @} */

/** @name Public function declarations */
/** @{ */

/**
 * @brief Initialises the ISO-TP library.
 *
 * @param link The @code IsoTpLink @endcode instance used for transceiving data.
 * @param sendid The ID used to send data to other CAN nodes.
 * @param sendbuf A pointer to an area in memory which can be used as a buffer for data to be
 * sent.
 * @param sendbufsize The size of the buffer area.
 * @param recvbuf A pointer to an area in memory which can be used as a buffer for data to be
 * received.
 * @param recvbufsize The size of the buffer area.
 */
void isotp_init_link(IsoTpLink* link, uint32_t sendid, uint8_t* sendbuf, uint32_t sendbufsize,
                     uint8_t* recvbuf, uint32_t recvbufsize);

/**
 * @brief Destroys the ISO-TP link and releases associated resources.
 *
 * @param link The @code IsoTpLink @endcode instance to destroy.
 */
void isotp_destroy_link(IsoTpLink* link);

/**
 * @brief Polling function; call this function periodically to handle timeouts, send consecutive
 * frames, etc.
 *
 * @param link The @code IsoTpLink @endcode instance used.
 */
void isotp_poll(IsoTpLink* link);

/**
 * @brief Handles incoming CAN messages.
 * Determines whether an incoming message is a valid ISO-TP frame or not and handles it
 * accordingly.
 *
 * @param link The @code IsoTpLink @endcode instance used for transceiving data.
 * @param data The data received via CAN.
 * @param len The length of the data received.
 */
void isotp_on_can_message(IsoTpLink* link, const uint8_t* data, uint8_t len);

/**
 * @brief Sends ISO-TP frames via CAN, using the ID set in the initialising function.
 *
 * Single-frame messages will be sent immediately when calling this function.
 * Multi-frame messages will be sent consecutively when calling isotp_poll.
 *
 * @param link The @code IsoTpLink @endcode instance used for transceiving data.
 * @param payload The payload to be sent. (Up to 4095 bytes).
 * @param size The size of the payload to be sent.
 *
 * @return Possible return values:
 *  - @code ISOTP_RET_OVERFLOW @endcode
 *  - @code ISOTP_RET_INPROGRESS @endcode
 *  - @code ISOTP_RET_OK @endcode
 *  - The return value of the user shim function isotp_user_send_can().
 */
int isotp_send(IsoTpLink* link, const uint8_t payload[], uint32_t size);

/**
 * @brief See @link isotp_send @endlink, with the exception that this function is used only for
 * functional addressing.
 */
int isotp_send_with_id(IsoTpLink* link, uint32_t id, const uint8_t payload[], uint32_t size);

/**
 * @brief Receives and parses the received data and copies the parsed data into the internal
 * buffer.
 * @param link The @link IsoTpLink @endlink instance used to transceive data.
 * @param payload A pointer to an area in memory where the raw data is copied from.
 * @param payload_size The size of the received (raw) CAN data.
 * @param out_size A reference to a variable which will contain the size of the actual (parsed)
 * data.
 *
 * @return Possible return values:
 *      - @link ISOTP_RET_OK @endlink
 *      - @link ISOTP_RET_NO_DATA @endlink
 */
int isotp_receive(IsoTpLink* link, uint8_t* payload, const uint32_t payload_size,
                  uint32_t* out_size);

#ifdef ISO_TP_TRANSMIT_COMPLETE_CALLBACK
/**
 * @brief Sets the callback function for transmission complete notification.
 *
 * @param link The @code IsoTpLink @endcode instance used for transceiving data.
 * @param cb The callback function to be called when transmission is complete.
 * @param arg A pointer that will be passed to the callback function.
 */
void isotp_set_tx_done_cb(IsoTpLink* link, isotp_tx_done_cb cb, void* arg);
#endif

#ifdef ISO_TP_RECEIVE_COMPLETE_CALLBACK
/**
 * @brief Sets the callback function for receive complete notification.
 *
 * @param link The @code IsoTpLink @endcode instance used for transceiving data.
 * @param cb The callback function to be called when a message is received.
 * @param arg A pointer that will be passed to the callback function.
 */
void isotp_set_rx_done_cb(IsoTpLink* link, isotp_rx_done_cb cb, void* arg);
#endif

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ISOTPC_H */
