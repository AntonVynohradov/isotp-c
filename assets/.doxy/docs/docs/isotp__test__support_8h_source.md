

# File isotp\_test\_support.h

[**File List**](files.md) **>** [**tests**](dir_59425e443f801f1f2fd8bbe4959a3ccf.md) **>** [**isotp\_test\_support.h**](isotp__test__support_8h.md)

[Go to the documentation of this file](isotp__test__support_8h.md)


```C++
/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: Test support utilities and mocks.
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

#ifndef ISOTP_TEST_SUPPORT_H
#define ISOTP_TEST_SUPPORT_H

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <cstdint>
#include <cstddef>
#include <cstring>

extern "C"
{
#include "isotp.h"
}

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/


/* ==============================================================================
 * TYPE DEFINITIONS
 * =============================================================================*/

struct MockCanState
{
    uint32_t last_id;
    uint8_t last_data[8];
    uint8_t last_size;
    int return_value;
    int call_count;
};

/* ==============================================================================
 * GLOBAL VARIABLES (extern declarations)
 * =============================================================================*/

extern MockCanState g_can_state;  
extern uint32_t g_now_us;         
extern int g_debug_call_count;    

/* ==============================================================================
 * PUBLIC FUNCTION DECLARATIONS
 * =============================================================================*/

void reset_mocks();

#endif  // ISOTP_TEST_SUPPORT_H
```


