/**
 * @file isotp_link.h
 * @brief Transport-independent ISO-TP link: segmentation, flow control and timing.
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

/**
 * @defgroup isotp_link Link
 * @ingroup isotp
 * @brief Transport layer of one ISO-TP connection: segmentation, flow control, timing.
 *
 * @details A link connects this node to one peer: it sends with the CAN ID
 *          @ref isotp_link_config::tx_id and receives the frames the application
 *          routes to it (normally the frames with the peer's CAN ID). The link
 *          never reads a clock or touches hardware. The integrator passes every
 *          received CAN frame to @ref isotp_link_receive, provides the frame writer
 *          @ref isotp_can_write_fn, and calls @ref isotp_link_poll regularly; the
 *          current time in microseconds is an argument of every call.
 *
 *          Transmission and reception are independent (full duplex). A complete
 *          received message is delivered to @ref isotp_link_config::on_message; the
 *          end of a transmission is reported to @ref isotp_link_config::on_tx_done.
 *
 *          A link is not thread-safe. Call @ref isotp_link_receive and
 *          @ref isotp_link_poll from one context (for example a CAN task, not an
 *          interrupt handler) and serialize @ref isotp_link_send with them.
 *
 * @par Public interface includes:
 * @li Lifecycle: @ref isotp_link_config_default, @ref isotp_link_init.
 * @li Message flow: @ref isotp_link_receive, @ref isotp_link_send,
 *     @ref isotp_link_send_functional, @ref isotp_link_poll.
 * @li Diagnostics: @ref isotp_link_tx_busy, @ref isotp_link_tx_result,
 *     @ref isotp_link_rx_result, @ref isotp_link_invalid_frames.
 * @{
 */

#ifndef ISOTP_LINK_H
#define ISOTP_LINK_H

/** @name Includes */
/** @{ */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "isotp_config.h"
#include "isotp_protocol.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @} */

/** @name Type definitions */
/** @{ */

/**
 * @brief Write one CAN frame to the bus.
 * @param context Value of isotp_link_config::write_context.
 * @param can_id CAN identifier of the frame.
 * @param data Frame data; valid only during the call.
 * @param length Number of data bytes, 1..8.
 * @retval 0 Frame queued for transmission.
 * @retval ISOTP_BUSY No space in the transmit queue; the link retries a
 *         Consecutive Frame on the next @ref isotp_link_poll.
 * @retval Other Driver error: the transmission ends with ISOTP_N_ERROR.
 */
typedef int (*isotp_can_write_fn)(void* context, uint32_t can_id, const uint8_t* data,
                                  size_t length);

/**
 * @brief Handle one complete received message.
 * @details The data is valid only for the duration of the callback; copy what
 *          must be kept. The handler may call @ref isotp_link_send on the same
 *          link to answer.
 * @param data Message bytes.
 * @param length Message length in bytes.
 * @param context Value of isotp_link_config::app_context.
 */
typedef void (*isotp_message_handler_t)(const uint8_t* data, uint32_t length, void* context);

/**
 * @brief Report the outcome of a transmission or a failed reception.
 * @param n_result ISOTP_N_* code.
 * @param context Value of isotp_link_config::app_context.
 */
typedef void (*isotp_result_fn)(int n_result, void* context);

/**
 * @brief Link configuration passed to @ref isotp_link_init.
 * @details Start from @ref isotp_link_config_default, then set the CAN ID, the
 *          writer and the callbacks. The link keeps a copy.
 */
struct isotp_link_config
{
    uint32_t tx_id;                     /**< CAN ID of every frame this link sends. */
    isotp_can_write_fn write;           /**< Frame writer (required). */
    void* write_context;                /**< Context for write. */
    isotp_message_handler_t on_message; /**< Received message; NULL discards it. */
    isotp_result_fn on_tx_done;         /**< End of a transmission; may be NULL. */
    isotp_result_fn on_rx_error;        /**< Failed reception; may be NULL. */
    void* app_context;                  /**< Context for the three callbacks above. */
    uint8_t block_size;                 /**< BS requested from the sender, 0 = unlimited. */
    uint32_t st_min_us;                 /**< STmin requested from the sender, 0..127000 us. */
    uint32_t n_bs_timeout_us;           /**< N_Bs, 1..2^31-1 us. */
    uint32_t n_cr_timeout_us;           /**< N_Cr, 1..2^31-1 us. */
    uint8_t wft_max;                    /**< N_WFTmax: Wait frames accepted in a row. */
    bool padding;                       /**< Pad every sent frame to 8 bytes. */
    uint8_t padding_value;              /**< Padding byte, used when padding is true. */
};

/** @brief Transmitter state. */
enum isotp_tx_state
{
    ISOTP_TX_IDLE = 0, /**< No transmission. */
    ISOTP_TX_WAIT_FC,  /**< First Frame or block sent, waiting for Flow Control. */
    ISOTP_TX_SEND_CF   /**< Sending Consecutive Frames. */
};

/** @brief Receiver state. */
enum isotp_rx_state
{
    ISOTP_RX_IDLE = 0, /**< No reception. */
    ISOTP_RX_RECEIVING /**< First Frame received, collecting Consecutive Frames. */
};

/** @brief Link state. Treat the members as private; use the functions below. */
struct isotp_link
{
    struct isotp_link_config config;         /**< Copy of the configuration. */
    uint8_t tx_buffer[ISOTP_TX_BUFFER_SIZE]; /**< Message being sent. */
    uint32_t tx_size;                        /**< Length of the message being sent. */
    uint32_t tx_offset;                      /**< Bytes already sent. */
    uint32_t tx_st_min_us;                   /**< STmin granted by the receiver. */
    uint32_t tx_deadline_us;                 /**< N_Bs expiry or time of the next CF. */
    enum isotp_tx_state tx_state;            /**< Transmitter state. */
    int tx_result;                           /**< N_Result of the last transmission. */
    uint8_t tx_sn;                           /**< SN of the next Consecutive Frame. */
    uint8_t tx_block_remaining;              /**< CFs left in the current block. */
    bool tx_block_unlimited;                 /**< Receiver granted BS = 0. */
    uint8_t tx_wft_count;                    /**< Wait frames received in a row. */
    uint8_t rx_buffer[ISOTP_RX_BUFFER_SIZE]; /**< Message being received. */
    uint32_t rx_size;                        /**< Announced message length. */
    uint32_t rx_offset;                      /**< Bytes already received. */
    uint32_t rx_deadline_us;                 /**< N_Cr expiry. */
    enum isotp_rx_state rx_state;            /**< Receiver state. */
    int rx_result;                           /**< N_Result of the last reception. */
    uint8_t rx_sn;                           /**< Expected SN. */
    uint8_t rx_block_count;                  /**< CFs left until the next Flow Control. */
    uint32_t invalid_frames;                 /**< Malformed frames ignored. */
};

/** @} */

/** @name Public function declarations */
/** @{ */

/**
 * @brief Fill a configuration with the defaults of isotp_config.h.
 * @details CAN ID 0, no writer, no callbacks, no padding (padding value 0xCC),
 *          ISOTP_DEFAULT_* flow control and timeouts.
 * @param config Configuration to fill; nothing happens when NULL.
 */
void isotp_link_config_default(struct isotp_link_config* config);

/**
 * @brief Initialize a link: copy the configuration, clear both directions.
 * @param link Link state to initialize.
 * @param config Configuration copied into the link.
 * @retval ISOTP_OK Link ready.
 * @retval ISOTP_ERROR NULL argument, no writer, or a value out of range.
 */
int isotp_link_init(struct isotp_link* link, const struct isotp_link_config* config);

/**
 * @brief Process one received CAN frame addressed to this link.
 * @details Single Frames and the last Consecutive Frame complete a message and
 *          call the message handler. A First Frame is answered with a Flow
 *          Control frame. Malformed frames are ignored and counted; unexpected
 *          Consecutive and Flow Control frames are ignored.
 * @param link Link state.
 * @param data CAN frame data.
 * @param length CAN data length, 1..8.
 * @param now_us Current time in microseconds (free-running, wraps at 2^32).
 */
void isotp_link_receive(struct isotp_link* link, const uint8_t* data, size_t length,
                        uint32_t now_us);

/**
 * @brief Start sending one message to the peer.
 * @details A message of up to 7 bytes is sent at once as a Single Frame and
 *          reported to on_tx_done before the call returns. A longer message is
 *          copied into the link, its First Frame is sent, and the rest follows
 *          from @ref isotp_link_poll as the receiver's flow control allows.
 * @param link Link state.
 * @param payload Message bytes.
 * @param length Message length, 1..ISOTP_TX_BUFFER_SIZE.
 * @param now_us Current time in microseconds.
 * @retval ISOTP_OK Message accepted.
 * @retval ISOTP_BUSY A segmented transmission is in progress, or the writer is busy.
 * @retval ISOTP_ERROR Invalid argument or message too long.
 * @retval Other Error returned by the writer.
 */
int isotp_link_send(struct isotp_link* link, const uint8_t* payload, uint32_t length,
                    uint32_t now_us);

/**
 * @brief Send a Single Frame with a functional (broadcast) CAN ID.
 * @details ISO 15765-2 allows only unsegmented messages with functional
 *          addressing. The frame is independent of the link's transmission
 *          state and does not call on_tx_done.
 * @param link Link state.
 * @param can_id Functional CAN ID.
 * @param payload Message bytes.
 * @param length Message length, 1..7.
 * @retval ISOTP_OK Frame written.
 * @retval ISOTP_ERROR Invalid argument or message longer than 7 bytes.
 * @retval Other Error returned by the writer.
 */
int isotp_link_send_functional(const struct isotp_link* link, uint32_t can_id,
                               const uint8_t* payload, uint32_t length);

/**
 * @brief Advance the timers: send the next Consecutive Frame, detect timeouts.
 * @details Sends at most one Consecutive Frame per call, when the separation
 *          time has passed. Call it at least as often as the shortest STmin you
 *          want to achieve (for example from a 1 ms tick or a loop).
 * @param link Link state.
 * @param now_us Current time in microseconds.
 */
void isotp_link_poll(struct isotp_link* link, uint32_t now_us);

/** @brief True while a segmented transmission is in progress (false for NULL). */
bool isotp_link_tx_busy(const struct isotp_link* link);

/** @brief N_Result of the last finished transmission (ISOTP_N_ERROR for NULL). */
int isotp_link_tx_result(const struct isotp_link* link);

/** @brief N_Result of the last finished or failed reception (ISOTP_N_ERROR for NULL). */
int isotp_link_rx_result(const struct isotp_link* link);

/** @brief Malformed frames ignored since @ref isotp_link_init (0 for NULL). */
uint32_t isotp_link_invalid_frames(const struct isotp_link* link);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* ISOTP_LINK_H */

/** @} */
