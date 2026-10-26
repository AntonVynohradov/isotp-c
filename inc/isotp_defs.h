/**
 * @file isotp_defs.h
 * @brief ISO 15765-2 protocol constants, result codes and big-endian helpers.
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
 * @defgroup isotp_defs Definitions
 * @ingroup isotp
 * @brief Protocol constants of ISO 15765-2 and the library result codes.
 *
 * @details Wire values (frame types, flow status, field limits) are taken from
 *          ISO 15765-2 for classical CAN (8 data bytes) with normal addressing.
 *          The result codes are library values: the standard defines the
 *          N_Result names, not their numbers.
 *
 * @par Elements in this group:
 * @li Function return codes: @ref ISOTP_OK, @ref ISOTP_ERROR, @ref ISOTP_BUSY.
 * @li Frame types (N_PCItype) and flow status values.
 * @li Frame layout limits: @ref ISOTP_CAN_DL, @ref ISOTP_SF_MAX_DL, ...
 * @li N_Result codes reported by the link: @ref ISOTP_N_OK, ...
 * @li Big-endian helpers: @ref isotp_get_u32_be, @ref isotp_put_u32_be.
 * @{
 */

#ifndef ISOTP_DEFS_H
#define ISOTP_DEFS_H

/** @name Includes */
/** @{ */

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @} */

/** @name Function return codes */
/** @{ */

#define ISOTP_OK 0     /**< Success. */
#define ISOTP_ERROR -1 /**< Invalid argument, malformed frame or insufficient capacity. */
#define ISOTP_BUSY -2  /**< Try again later: transmission in progress or CAN driver busy. */

/** @} */

/** @name Frame types (N_PCItype, high nibble of the first byte) */
/** @{ */

#define ISOTP_PCI_SINGLE_FRAME 0x0U      /**< SF: complete message of up to 7 bytes. */
#define ISOTP_PCI_FIRST_FRAME 0x1U       /**< FF: start of a segmented message. */
#define ISOTP_PCI_CONSECUTIVE_FRAME 0x2U /**< CF: next segment of a message. */
#define ISOTP_PCI_FLOW_CONTROL 0x3U      /**< FC: receiver tells the sender how to continue. */

/** @} */

/** @name Flow status (FS, low nibble of a Flow Control frame) */
/** @{ */

#define ISOTP_FS_CONTINUE 0x0U /**< CTS: continue to send. */
#define ISOTP_FS_WAIT 0x1U     /**< WAIT: wait for the next Flow Control frame. */
#define ISOTP_FS_OVERFLOW 0x2U /**< OVFLW: message longer than the receiver buffer. */

/** @} */

/** @name Frame layout (classical CAN, normal addressing) */
/** @{ */

#define ISOTP_CAN_DL 8U             /**< Data bytes of a classical CAN frame. */
#define ISOTP_SF_MAX_DL 7U          /**< Largest Single Frame payload. */
#define ISOTP_FF_DL_12BIT_MAX 4095U /**< Largest length in the 12-bit FF_DL field. */
#define ISOTP_FF_SHORT_DATA 6U      /**< Payload bytes in a First Frame with 12-bit FF_DL. */
#define ISOTP_FF_LONG_DATA 2U       /**< Payload bytes in a First Frame with 32-bit FF_DL. */
#define ISOTP_CF_MAX_DATA 7U        /**< Payload bytes in a full Consecutive Frame. */
#define ISOTP_FC_DL 3U              /**< Bytes of a Flow Control frame without padding. */
#define ISOTP_SN_MASK 0x0FU         /**< Sequence numbers count 0..15 and wrap. */
#define ISOTP_FIRST_SN 1U           /**< SN of the first Consecutive Frame. */
#define ISOTP_ST_MIN_MAX_US 127000U /**< Largest STmin, 0x7F = 127 ms. */

/** @} */

/**
 * @name N_Result codes
 * @details Outcome of a transmission or reception, reported by
 *          @ref isotp_link_tx_result, @ref isotp_link_rx_result and the
 *          completion callbacks. The names follow ISO 15765-2.
 */
/** @{ */

#define ISOTP_N_OK 0            /**< Completed successfully. */
#define ISOTP_N_TIMEOUT_A -1    /**< N_As/N_Ar expired (not produced: writes are synchronous). */
#define ISOTP_N_TIMEOUT_BS -2   /**< Sender: no Flow Control within N_Bs. */
#define ISOTP_N_TIMEOUT_CR -3   /**< Receiver: no Consecutive Frame within N_Cr. */
#define ISOTP_N_WRONG_SN -4     /**< Receiver: Consecutive Frame with an unexpected SN. */
#define ISOTP_N_INVALID_FS -5   /**< Sender: Flow Control with an unknown flow status. */
#define ISOTP_N_UNEXP_PDU -6    /**< Receiver: new SF/FF interrupted a reception. */
#define ISOTP_N_WFT_OVRN -7     /**< Sender: more than N_WFTmax Wait frames. */
#define ISOTP_N_BUFFER_OVFLW -8 /**< Message longer than the receiver buffer. */
#define ISOTP_N_ERROR -9        /**< CAN driver error while sending a frame. */

/** @} */

/** @name Big-endian helpers (the 32-bit FF_DL is sent most significant byte first) */
/** @{ */

/**
 * @brief Read a big-endian 32-bit value.
 * @param data At least four bytes.
 * @return The value.
 */
static inline uint32_t isotp_get_u32_be(const uint8_t* data)
{
    return (((uint32_t) data[0]) << 24U) | (((uint32_t) data[1]) << 16U)
           | (((uint32_t) data[2]) << 8U) | ((uint32_t) data[3]);
}

/**
 * @brief Write a 32-bit value in big-endian byte order.
 * @param data At least four bytes.
 * @param value Value to write.
 */
static inline void isotp_put_u32_be(uint8_t* data, uint32_t value)
{
    data[0] = (uint8_t) (value >> 24U);
    data[1] = (uint8_t) (value >> 16U);
    data[2] = (uint8_t) (value >> 8U);
    data[3] = (uint8_t) value;
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* ISOTP_DEFS_H */

/** @} */
