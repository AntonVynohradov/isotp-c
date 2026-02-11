/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: ISO-TP configuration parameters and feature toggles
 *
 * Author:      Anton Vynohradov
 * Email:       avynohradovair@gmail.com
 *
 * License:     MIT License
 *
 * Copyright (c) 2026 Anton Vynohradov
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
 ******************************************************************************/

#ifndef ISOTPC_CONFIG_H
#define ISOTPC_CONFIG_H

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

/* Max number of messages the receiver can receive at one time, this value
 * is affected by can driver queue length
 */
#ifndef ISO_TP_DEFAULT_BLOCK_SIZE
#define ISO_TP_DEFAULT_BLOCK_SIZE 8
#endif

/* The STmin parameter value specifies the minimum time gap allowed between
 * the transmission of consecutive frame network protocol data units
 */
#ifndef ISO_TP_DEFAULT_ST_MIN_US
#define ISO_TP_DEFAULT_ST_MIN_US 0
#endif

/* This parameter indicate how many FC N_PDU WTs can be transmitted by the
 * receiver in a row.
 */
#ifndef ISO_TP_MAX_WFT_NUMBER
#define ISO_TP_MAX_WFT_NUMBER 1
#endif

/* Private: The default timeout to use when waiting for a response during a
 * multi-frame send or receive.
 */
#ifndef ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US
#define ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US 100000
#endif

/* Private: Determines if by default, padding is added to ISO-TP message frames.
 */
/* #define ISO_TP_FRAME_PADDING */

/* Private: Value to use when padding frames if enabled by ISO_TP_FRAME_PADDING
 */
#ifndef ISO_TP_FRAME_PADDING_VALUE
#define ISO_TP_FRAME_PADDING_VALUE 0xAA
#endif

/* Private: Determines if by default, an additional argument is present in the
 * definition of isotp_user_send_can.
 */
/* #define ISO_TP_USER_SEND_CAN_ARG */

/* Enable support for transmission complete callback */
/* #define ISO_TP_TRANSMIT_COMPLETE_CALLBACK */

/* Enable support for receive complete callback */
/* #define ISO_TP_RECEIVE_COMPLETE_CALLBACK */

/* ==============================================================================
 * TYPE DEFINITIONS
 * =============================================================================*/

/* ==============================================================================
 * GLOBAL VARIABLES (extern declarations)
 * =============================================================================*/

/* ==============================================================================
 * PUBLIC FUNCTION DECLARATIONS
 * =============================================================================*/

#endif /* ISOTPC_CONFIG_H */
