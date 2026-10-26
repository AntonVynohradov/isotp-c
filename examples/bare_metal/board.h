/**
 * @file board.h
 * @brief Board interface of the bare-metal example: four functions to port per MCU.
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

#ifndef BOARD_H
#define BOARD_H

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <stddef.h>
#include <stdint.h>

/* ==============================================================================
 * PUBLIC FUNCTION DECLARATIONS
 * =============================================================================*/

/**
 * @brief   Configure the CAN controller, its acceptance filter and the RX interrupt.
 * @param   bitrate - CAN bit rate in bit/s
 */
void board_init(uint32_t bitrate);

/**
 * @brief   Queue one CAN frame for transmission (standard 11-bit identifier).
 * @param   can_id - CAN identifier
 * @param   data - Frame data
 * @param   length - Data length, 0..8
 * @return  0 when queued, ISOTP_BUSY when all TX mailboxes are full, other negative on error
 */
int board_can_write(uint32_t can_id, const uint8_t* data, size_t length);

/**
 * @brief   Free-running microsecond counter (wraps at 2^32).
 */
uint32_t board_micros(void);

/**
 * @brief   Wait for the next interrupt (for example WFI on Cortex-M).
 * @details Must return at least once per millisecond (for example on a SysTick
 *          interrupt), so that the main loop can poll the link for STmin and timeouts.
 */
void board_idle(void);

/**
 * @brief   Called by the board's CAN RX interrupt for every received frame.
 * @details Implemented by the application (main.c).
 */
void app_can_rx_isr(uint32_t can_id, const uint8_t* data, uint8_t length);

#endif /* BOARD_H */
