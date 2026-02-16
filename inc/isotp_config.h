/**
 * @file isotp_config.h
 * @brief ISO-TP configuration parameters and feature toggles.
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
 * @defgroup isotp ISO-TP API
 * @brief Public API for ISO-TP link management and message handling.
 * @details This module groups the public interface and its supporting
 *          submodules: @ref isotp_config, @ref isotp_defines, and
 *          @ref isotp_user.
 */

/**
 * @defgroup isotp_config Configuration
 * @ingroup isotp
 * @brief Build-time configuration for ISO-TP.
 *
 * @details Configuration parameters in this group control protocol behavior as exposed by the API
 * in isotp.h and implemented in isotp.c. They cover limits and timing used
 * when sending and receiving single- and multi-frame messages, including
 * block size (BS), minimum separation time (ST_min), and response timeouts.
 * They also enable optional features such as frame padding, an extra argument
 * for isotp_user_send_can, and callbacks for transmit and receive completion.
 *
 * @par Configuration elements in this group:
 * @li @ref ISO_TP_DEFAULT_BLOCK_SIZE : default receiver block size (BS).
 * @li @ref ISO_TP_DEFAULT_ST_MIN_US : default minimum separation time (ST_min) in us.
 * @li @ref ISO_TP_MAX_WFT_NUMBER : maximum number of FC.Wait frames in a row.
 * @li @ref ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US : response timeout during multi-frame transfer.
 * @li @ref ISO_TP_FRAME_PADDING : enable frame padding for ISO-TP frames.
 * @li @ref ISO_TP_FRAME_PADDING_VALUE : padding byte value when padding is enabled.
 * @li @ref ISO_TP_USER_SEND_CAN_ARG : add a user argument to @ref isotp_user_send_can.
 * @li @ref ISO_TP_TRANSMIT_COMPLETE_CALLBACK : enable transmit complete callback support.
 * @li @ref ISO_TP_RECEIVE_COMPLETE_CALLBACK: enable receive complete callback support.
 * @internal
 * @{
 */

#ifndef ISOTPC_CONFIG_H
#define ISOTPC_CONFIG_H

/** @name Includes */
/** @{ */
/** @} */

/** @name Defines and macros */
/** @{ */

/**
 * @def ISO_TP_DEFAULT_BLOCK_SIZE
 * @brief Max number of messages the receiver can receive at one time.
 *
 * This value is affected by CAN driver queue length.
 */
#ifndef ISO_TP_DEFAULT_BLOCK_SIZE
#define ISO_TP_DEFAULT_BLOCK_SIZE (8)
#endif

/**
 * @def ISO_TP_DEFAULT_ST_MIN_US
 * @brief Minimum time gap allowed between consecutive frame N_PDUs.
 */
#ifndef ISO_TP_DEFAULT_ST_MIN_US
#define ISO_TP_DEFAULT_ST_MIN_US (0)
#endif

/**
 * @def ISO_TP_MAX_WFT_NUMBER
 * @brief Number of FC N_PDU WTs that can be transmitted by the receiver in a row.
 */
#ifndef ISO_TP_MAX_WFT_NUMBER
#define ISO_TP_MAX_WFT_NUMBER (1)
#endif

/**
 * @def ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US
 * @brief Default timeout while waiting for a response during a multi-frame transfer.
 * @internal
 */
#ifndef ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US
#define ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US (100000)
#endif

/**
 * @def ISO_TP_FRAME_PADDING
 * @brief Enables padding of ISO-TP message frames by default.
 * @internal
 */
/* #define ISO_TP_FRAME_PADDING */

/**
 * @def ISO_TP_FRAME_PADDING_VALUE
 * @brief Padding byte value when ISO_TP_FRAME_PADDING is enabled.
 * @internal
 */
#ifndef ISO_TP_FRAME_PADDING_VALUE
#define ISO_TP_FRAME_PADDING_VALUE (0xAA)
#endif

/**
 * @def ISO_TP_USER_SEND_CAN_ARG
 * @brief Adds an extra argument to `isotp_user_send_can` by default.
 * @internal
 */
/* #define ISO_TP_USER_SEND_CAN_ARG */

/**
 * @def ISO_TP_TRANSMIT_COMPLETE_CALLBACK
 * @brief Enables support for transmission complete callback.
 */
/* #define ISO_TP_TRANSMIT_COMPLETE_CALLBACK */

/**
 * @def ISO_TP_RECEIVE_COMPLETE_CALLBACK
 * @brief Enables support for receive complete callback.
 */
/* #define ISO_TP_RECEIVE_COMPLETE_CALLBACK */

/** @} */

/** @name Type definitions */
/** @{ */
/** @} */

/** @name Global variables */
/** @{ */
/** @} */

/** @name Public function declarations */
/** @{ */
/** @} */

/** @} */

#endif /* ISOTPC_CONFIG_H */
