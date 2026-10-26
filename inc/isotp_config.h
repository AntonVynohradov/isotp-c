/**
 * @file isotp_config.h
 * @brief Build-time configuration: buffer sizes and protocol defaults.
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
 * @defgroup isotp ISOTP API
 * @brief Public API of the ISO 15765-2 (ISO-TP) library.
 * @details This module groups the public interface and its supporting
 *          submodules: @ref isotp_config, @ref isotp_defs, @ref isotp_protocol
 *          and @ref isotp_link.
 */

/**
 * @defgroup isotp_config Configuration
 * @ingroup isotp
 * @brief Build-time configuration for ISOTP-C.
 *
 * @details Every parameter can be overridden by the integrator with a compiler
 *          definition (for example `-DISOTP_RX_BUFFER_SIZE=64U`) without editing
 *          this file. The buffer sizes are compiled into @ref isotp_link, so they
 *          set the RAM footprint of one link. The protocol defaults are only the
 *          values written by @ref isotp_link_config_default; every link can use
 *          its own values at run time.
 *
 * @par Configuration elements in this group:
 * @li @ref ISOTP_TX_BUFFER_SIZE, @ref ISOTP_RX_BUFFER_SIZE : largest message in each direction.
 * @li @ref ISOTP_DEFAULT_BLOCK_SIZE, @ref ISOTP_DEFAULT_ST_MIN_US : flow control we request.
 * @li @ref ISOTP_DEFAULT_N_BS_TIMEOUT_US, @ref ISOTP_DEFAULT_N_CR_TIMEOUT_US : timeouts.
 * @li @ref ISOTP_DEFAULT_WFT_MAX : flow-control wait frames we accept.
 * @{
 */

#ifndef ISOTP_CONFIG_H
#define ISOTP_CONFIG_H

/** @name Defines and macros */
/** @{ */

/**
 * @def ISOTP_TX_BUFFER_SIZE
 * @brief Largest message one link can transmit, in bytes.
 * @details The link copies the message into a buffer of this size, so the caller
 *          may reuse its own buffer as soon as @ref isotp_link_send returns.
 */
#ifndef ISOTP_TX_BUFFER_SIZE
#define ISOTP_TX_BUFFER_SIZE 4095U
#endif

/**
 * @def ISOTP_RX_BUFFER_SIZE
 * @brief Largest message one link can receive, in bytes.
 * @details A First Frame announcing a longer message is answered with a Flow
 *          Control frame with the status Overflow.
 */
#ifndef ISOTP_RX_BUFFER_SIZE
#define ISOTP_RX_BUFFER_SIZE 4095U
#endif

/**
 * @def ISOTP_DEFAULT_BLOCK_SIZE
 * @brief Block size (BS) this receiver requests; 0 means no further Flow Control.
 */
#ifndef ISOTP_DEFAULT_BLOCK_SIZE
#define ISOTP_DEFAULT_BLOCK_SIZE 8U
#endif

/**
 * @def ISOTP_DEFAULT_ST_MIN_US
 * @brief Minimum separation time (STmin) this receiver requests, in microseconds.
 */
#ifndef ISOTP_DEFAULT_ST_MIN_US
#define ISOTP_DEFAULT_ST_MIN_US 0U
#endif

/**
 * @def ISOTP_DEFAULT_N_BS_TIMEOUT_US
 * @brief Sender timeout N_Bs: longest wait for a Flow Control frame, in microseconds.
 * @details ISO 15765-2 specifies 1000 ms.
 */
#ifndef ISOTP_DEFAULT_N_BS_TIMEOUT_US
#define ISOTP_DEFAULT_N_BS_TIMEOUT_US 1000000U
#endif

/**
 * @def ISOTP_DEFAULT_N_CR_TIMEOUT_US
 * @brief Receiver timeout N_Cr: longest wait for the next Consecutive Frame, in microseconds.
 * @details ISO 15765-2 specifies 1000 ms.
 */
#ifndef ISOTP_DEFAULT_N_CR_TIMEOUT_US
#define ISOTP_DEFAULT_N_CR_TIMEOUT_US 1000000U
#endif

/**
 * @def ISOTP_DEFAULT_WFT_MAX
 * @brief N_WFTmax: Flow Control frames with the status Wait accepted in a row.
 */
#ifndef ISOTP_DEFAULT_WFT_MAX
#define ISOTP_DEFAULT_WFT_MAX 10U
#endif

/*
 * A buffer holds at least one segmented message (8 bytes is the smallest) and
 * stays below 2^31, so every size and offset fits a uint32_t with headroom.
 */
#if (ISOTP_TX_BUFFER_SIZE < 8U) || (ISOTP_TX_BUFFER_SIZE > 0x7FFFFFFFU)
#error "ISOTP_TX_BUFFER_SIZE must be in the range 8..2147483647"
#endif

#if (ISOTP_RX_BUFFER_SIZE < 8U) || (ISOTP_RX_BUFFER_SIZE > 0x7FFFFFFFU)
#error "ISOTP_RX_BUFFER_SIZE must be in the range 8..2147483647"
#endif

#if (ISOTP_DEFAULT_BLOCK_SIZE > 255U)
#error "ISOTP_DEFAULT_BLOCK_SIZE must be in the range 0..255"
#endif

#if (ISOTP_DEFAULT_ST_MIN_US > 127000U)
#error "ISOTP_DEFAULT_ST_MIN_US must be in the range 0..127000"
#endif

#if (ISOTP_DEFAULT_N_BS_TIMEOUT_US < 1U) || (ISOTP_DEFAULT_N_BS_TIMEOUT_US > 0x7FFFFFFFU)
#error "ISOTP_DEFAULT_N_BS_TIMEOUT_US must be in the range 1..2147483647"
#endif

#if (ISOTP_DEFAULT_N_CR_TIMEOUT_US < 1U) || (ISOTP_DEFAULT_N_CR_TIMEOUT_US > 0x7FFFFFFFU)
#error "ISOTP_DEFAULT_N_CR_TIMEOUT_US must be in the range 1..2147483647"
#endif

#if (ISOTP_DEFAULT_WFT_MAX > 255U)
#error "ISOTP_DEFAULT_WFT_MAX must be in the range 0..255"
#endif

/** @} */

#endif /* ISOTP_CONFIG_H */

/** @} */
