

# File mock\_can.h

[**File List**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**python\_api**](dir_181ea403426f4b233011860116dcb1b5.md) **>** [**inc**](dir_a048739b84e6d1db49a725d8f1a9c5e6.md) **>** [**mock\_can.h**](mock__can_8h.md)

[Go to the documentation of this file](mock__can_8h.md)


```C++
/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: The mock CAN driver implementation for testing purposes. This file provides a simple
 *              in-memory queue to simulate CAN communication, allowing for testing of the ISO-TP
 *              implementation without requiring actual CAN hardware. It supports basic send and
 *              receive operations, as well as optional frame dropping and artificial delays.
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

#ifndef MOCK_CAN_H
#define MOCK_CAN_H

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

void mock_can_init(void);

void mock_can_enable_drop(int enable);

void mock_can_disable_fc(int enable);

void mock_can_set_delay_ms(int delay);

#ifdef __cplusplus
}
#endif

#endif /* MOCK_CAN_H */
```


