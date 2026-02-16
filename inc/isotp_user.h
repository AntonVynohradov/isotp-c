/**
 * @file isotp_user.h
 * @brief User callback interfaces and platform abstraction layer.
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
 * @defgroup isotp_user User hooks
 * @ingroup isotp
 * @brief User-implemented callbacks and platform hooks.
 *
 * This group defines the platform abstraction points required by the ISO-TP
 * core. Applications must provide these functions to integrate with their
 * CAN driver, timing source, and optional debug output.
 *
 * @par Elements in this group include:
 * @li Debug output hook: @ref isotp_user_debug.
 * @li CAN transmit hook: @ref isotp_user_send_can (optionally with
 *     @ref ISO_TP_USER_SEND_CAN_ARG).
 * @li Time source hook: @ref isotp_user_get_us.
 * @{
 */

#ifndef ISOTPC_USER_H
#define ISOTPC_USER_H

/** @name Includes */
/** @{ */

#include <stdint.h>

/** @} */

/** @name Defines and macros */
/** @{ */
/** @} */

/** @name Type definitions */
/** @{ */
/** @} */

/** @name Global variables */
/** @{ */
/** @} */

/** @name Public function declarations */
/** @{ */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief User implemented, print debug message.
 * @param message Debug message format string.
 */
void isotp_user_debug(const char* message, ...);

/**
 * @brief User implemented, send CAN message.
 * @details Should return ISOTP_RET_OK on success.
 * @param arbitration_id CAN message arbitration ID.
 * @param data Pointer to message data buffer.
 * @param size Size of message data in bytes.
 * @return ISOTP_RET_OK on success, ISOTP_RET_NOSPACE if transfer should be retried later,
 *         or ISOTP_RET_ERROR on failure.
 */
int isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size
#ifdef ISO_TP_USER_SEND_CAN_ARG
                        ,
                        void* arg
#endif
);

/**
 * @brief User implemented, return a monotonically increasing timebase in microseconds.
 *
 * This function must provide a free-running timestamp in microseconds that
 * increases monotonically over time. The ISO-TP core uses this value for
 * timeout and delay calculations by performing arithmetic on the returned
 * timestamp (for example, adding a timeout interval to it).
 *
 * Natural wrap-around of the 32-bit counter is allowed and will be handled
 * by the ISO-TP core, but the value must not jump backwards except for such
 * wrap-around.
 *
 * @return Current timebase value in microseconds.
 */
uint32_t isotp_user_get_us(void);

/** @} */

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ISOTPC_USER_H */
