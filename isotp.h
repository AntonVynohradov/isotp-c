/**
 * @file isotp.h
 * @brief Umbrella header of the ISO 15765-2 (ISO-TP) library.
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

/**
 * @addtogroup isotp ISOTP API
 * @brief Public API of the ISO 15765-2 (ISO-TP) library.
 *
 * @details Include this single header to use the library:
 * @li isotp_config.h   build-time configuration (buffer sizes, protocol defaults)
 * @li isotp_defs.h     frame types, flow status, N_Result codes, big-endian helpers
 * @li isotp_protocol.h frame codec: decode, encode SF/FF/CF/FC, STmin
 * @li isotp_link.h     transport layer of one connection (segmentation, flow control, timing)
 * @{
 */

#ifndef ISOTP_H
#define ISOTP_H

/** @name Includes */
/** @{ */

#include "isotp_config.h"
#include "isotp_defs.h"
#include "isotp_protocol.h"
#include "isotp_link.h"

/** @} */

/**
 * @name Library version
 * @details Normally injected by cmake/version.cmake from the Git tag; the
 *          values below apply to builds that do not use CMake.
 */
/** @{ */

#ifndef ISOTP_VERSION_MAJOR
#define ISOTP_VERSION_MAJOR 2
#endif

#ifndef ISOTP_VERSION_MINOR
#define ISOTP_VERSION_MINOR 0
#endif

#ifndef ISOTP_VERSION_PATCH
#define ISOTP_VERSION_PATCH 1
#endif

/** Version string "MAJOR.MINOR.PATCH". */
#ifndef ISOTP_VERSION
#define ISOTP_VERSION "2.0.1"
#endif

/** @} */

#endif /* ISOTP_H */

/** @} */
