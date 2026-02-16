/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: Unit tests for isotp_destroy_link
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

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <gtest/gtest.h>

#include "isotp_test_support.h"

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

/* Macros and constants for this test file */

/* ==============================================================================
 * PRIVATE TYPE DEFINITIONS
 * =============================================================================*/

/* Local type definitions */

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

/* static variables */

/* ==============================================================================
 * PRIVATE FUNCTION DECLARATIONS (static)
 * =============================================================================*/

/* static helpers */

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

/* static helper implementations */

/* ==============================================================================
 * UNIT TEST IMPLEMENTATIONS
 * =============================================================================*/

TEST(IsotpDestroyLink, ClearsState)
{
    IsoTpLink link;
    std::memset(&link, 0x5A, sizeof(link));

    isotp_destroy_link(&link);

    IsoTpLink zeroed;
    std::memset(&zeroed, 0, sizeof(zeroed));

    EXPECT_EQ(std::memcmp(&link, &zeroed, sizeof(link)), 0);
}

TEST(IsotpDestroyLink, NullIsNoOp)
{
    EXPECT_NO_FATAL_FAILURE(isotp_destroy_link(NULL));
}
