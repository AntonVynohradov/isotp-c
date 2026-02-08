

# File isotp\_user.h

[**File List**](files.md) **>** [**Inc**](dir_f93dfce691d792349f1c14ede440e2e6.md) **>** [**isotp\_user.h**](isotp__user_8h.md)

[Go to the documentation of this file](isotp__user_8h.md)


```C++
/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: User callback interfaces and platform abstraction layer
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

#ifndef ISOTPC_USER_H
#define ISOTPC_USER_H

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <stdint.h>

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

/* ==============================================================================
 * TYPE DEFINITIONS
 * =============================================================================*/

/* ==============================================================================
 * GLOBAL VARIABLES (extern declarations)
 * =============================================================================*/

/* ==============================================================================
 * PUBLIC FUNCTION DECLARATIONS
 * =============================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif

    void isotp_user_debug(const char* message, ...);

    int isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size
#ifdef ISO_TP_USER_SEND_CAN_ARG
                            ,
                            void* arg
#endif
    );

    uint32_t isotp_user_get_us(void);

#ifdef __cplusplus
}
#endif

#endif /* ISOTPC_USER_H */
```


