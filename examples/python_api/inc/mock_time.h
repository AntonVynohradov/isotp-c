/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: File description - briefly describe purpose of this header
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

#ifndef MOCK_TIME_H
#define MOCK_TIME_H

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

/* Define your types, enums, and structs here */

/* ==============================================================================
 * GLOBAL VARIABLES (extern declarations)
 * =============================================================================*/

/* extern declarations for global variables */

/* ==============================================================================
 * PUBLIC FUNCTION DECLARATIONS
 * =============================================================================*/

/**
 * @brief Returns the current virtual time in microseconds.
 *
 * @return uint32_t Current virtual time in microseconds.
 */
uint32_t mock_time_now(void);

/**
 * @brief Advances the virtual time by the specified number of microseconds.
 * @param delta - Number of microseconds to advance the virtual time.
 */
void mock_time_advance(uint32_t delta);

/**
 * @brief Sets the virtual time to a specific value.
 * @param value - The value to set the virtual time to, in microseconds.
 */
void mock_time_set(uint32_t value);

/**
 * @brief Resets the virtual time to zero.
 */
void mock_time_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* MOCK_TIME_H */
