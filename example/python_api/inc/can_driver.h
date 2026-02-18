/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: The CAN driver interface definition for the ISO-TP implementation. This header
 *              defines the structure and functions required to interact with a CAN driver. It
 *              allows for abstraction of the CAN communication layer, enabling the use of different
 *              CAN drivers without modifying the ISO-TP implementation. The mock CAN driver
 *              provided in this project implements this interface for testing purposes.
 *
 * Author:      Anton Vynohradov
 * Email:       avynohradov@systemfromscratch.com
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

#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

/* Define your macros and constants here */

/* ==============================================================================
 * TYPE DEFINITIONS
 * =============================================================================*/

typedef struct
{
    int (*send)(uint32_t id, const uint8_t* data, uint8_t len);
    int (*receive)(uint32_t* id, uint8_t* data, uint8_t* len);
} can_driver_t;

/* ==============================================================================
 * GLOBAL VARIABLES (extern declarations)
 * =============================================================================*/

/* extern declarations for global variables */

/* ==============================================================================
 * PUBLIC FUNCTION DECLARATIONS
 * =============================================================================*/

/**
 * @brief Sets the active CAN driver to be used by the ISO-TP implementation.
 *
 * @param driver Pointer to the CAN driver to set as active.
 */
void can_set_driver(const can_driver_t* driver);

/**
 * @brief Sends a CAN frame using the active CAN driver.
 *
 * @param id CAN ID to send.
 * @param data Pointer to the CAN data to send.
 * @param len Length of the CAN data to send.
 * @return int 0 on success, -1 on failure (e.g., if the driver is not set or if send fails).
 */
int can_send(uint32_t id, const uint8_t* data, uint8_t len);

/**
 * @brief Receives a CAN frame using the active CAN driver.
 *
 * @param id Pointer to store the received CAN ID.
 * @param data Pointer to buffer to store the received CAN data.
 * @param len Pointer to variable to store the length of the received CAN data.
 * @return int 0 on success, -1 if the queue is empty or if the driver is not set.
 */
int can_receive(uint32_t* id, uint8_t* data, uint8_t* len);

#ifdef __cplusplus
}
#endif

#endif /* CAN_DRIVER_H */
