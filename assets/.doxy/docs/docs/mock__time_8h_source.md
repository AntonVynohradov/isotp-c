

# File mock\_time.h

[**File List**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**python\_api**](dir_181ea403426f4b233011860116dcb1b5.md) **>** [**inc**](dir_a048739b84e6d1db49a725d8f1a9c5e6.md) **>** [**mock\_time.h**](mock__time_8h.md)

[Go to the documentation of this file](mock__time_8h.md)


```C++
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

uint32_t mock_time_now(void);

void mock_time_advance(uint32_t delta);

void mock_time_set(uint32_t value);

void mock_time_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* MOCK_TIME_H */
```


