/**
 * @file isotp_defines.h
 * @brief Internal protocol definitions and type structures for ISO-TP.
 *
 * Project: ISO-TP-C - Embedded-Grade Refactoring & Optimization
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
 * @defgroup isotp_defines Protocol definitions
 * @ingroup isotp
 * @brief Internal protocol definitions and type structures.
 *
 * This group contains the low-level building blocks used by the ISO-TP core
 * (frame layouts, PCI types, status codes, and timing helpers). These
 * definitions describe how CAN frames are encoded/decoded and provide enums
 * and constants that drive the state machine in isotp.c.
 *
 * @par Elements in this group include:
 * @li Frame layout structs: @ref IsoTpSingleFrame, @ref IsoTpFirstFrameShort,
 *     @ref IsoTpFirstFrameLong, @ref IsoTpConsecutiveFrame, @ref IsoTpFlowControl.
 * @li Frame container: @ref IsoTpCanMessage and @ref IsoTpDataArray.
 * @li PCI type and flow status enums: @ref IsoTpProtocolControlInformation,
 *     @ref IsoTpFlowStatus.
 * @li Return codes and protocol result codes: @ref ISOTP_RET_OK,
 *     @ref ISOTP_RET_ERROR, @ref ISOTP_RET_INPROGRESS, @ref ISOTP_RET_OVERFLOW,
 *     @ref ISOTP_RET_WRONG_SN, @ref ISOTP_RET_NO_DATA, @ref ISOTP_RET_TIMEOUT,
 *     @ref ISOTP_RET_LENGTH, @ref ISOTP_RET_NOSPACE,
 *     @ref ISOTP_PROTOCOL_RESULT_OK, @ref ISOTP_PROTOCOL_RESULT_TIMEOUT_A,
 *     @ref ISOTP_PROTOCOL_RESULT_TIMEOUT_BS, @ref ISOTP_PROTOCOL_RESULT_TIMEOUT_CR,
 *     @ref ISOTP_PROTOCOL_RESULT_WRONG_SN, @ref ISOTP_PROTOCOL_RESULT_INVALID_FS,
 *     @ref ISOTP_PROTOCOL_RESULT_UNEXP_PDU, @ref ISOTP_PROTOCOL_RESULT_WFT_OVRN,
 *     @ref ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW, @ref ISOTP_PROTOCOL_RESULT_ERROR.
 * @li Byte order and packing helpers: @ref ISOTP_BYTE_ORDER_LITTLE_ENDIAN,
 *     @ref ISOTP_PACKED_STRUCT, @ref LE32TOH.
 * @li Status types and helpers: @ref IsoTpSendStatusTypes,
 *     @ref IsoTpReceiveStatusTypes, @ref IsoTpTimeAfter, @ref ISOTP_INVALID_BS,
 *     @ref ISOTP_MAX_ERROR_MSG_SIZE.
 * @internal
 * @{
 */

#ifndef ISOTPC_USER_DEFINITIONS_H
#define ISOTPC_USER_DEFINITIONS_H

/** @name Includes */
/** @{ */

#include <stdint.h>

/** @} */

/** @name Defines and macros */
/** @{ */

/** @brief Compiler-specific defines. */
#ifdef __GNUC__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ISOTP_BYTE_ORDER_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#else
#error "unsupported byte ordering"
#endif

#define ISOTP_PACKED_STRUCT(content) typedef struct __attribute__((packed)) content
#endif

/** @brief OS-specific defines. */
#ifdef _MSC_VER
#define ISOTP_PACKED_STRUCT(content) \
    __pragma(pack(push, 1)) typedef struct content __pragma(pack(pop))

#define snprintf _snprintf

#include <windows.h>
#define ISOTP_BYTE_ORDER_LITTLE_ENDIAN
#define __builtin_bswap8 _byteswap_uint8
#define __builtin_bswap16 _byteswap_uint16
#define __builtin_bswap32 _byteswap_uint32
#define __builtin_bswap64 _byteswap_uint64
#endif

/** @brief Convert a 32-bit value from little-endian to host byte order. */
#define LE32TOH(le)                                                                     \
    ((uint32_t) (((le) << 24) | (((le) & 0x0000FF00) << 8) | (((le) & 0x00FF0000) >> 8) \
                 | ((le) >> 24)))

/** @brief Internal return codes. */
#define ISOTP_RET_OK 0
#define ISOTP_RET_ERROR -1
#define ISOTP_RET_INPROGRESS -2
#define ISOTP_RET_OVERFLOW -3
#define ISOTP_RET_WRONG_SN -4
#define ISOTP_RET_NO_DATA -5
#define ISOTP_RET_TIMEOUT -6
#define ISOTP_RET_LENGTH -7
#define ISOTP_RET_NOSPACE -8

/** @brief Returns true if time `a` is after time `b`. */
#define IsoTpTimeAfter(a, b) ((int32_t) ((int32_t) (b) - (int32_t) (a)) < 0)

/** @brief Invalid block size marker. */
#define ISOTP_INVALID_BS 0xFFFF

/** @brief Max number of characters allowed in an error message. */
#define ISOTP_MAX_ERROR_MSG_SIZE 128

/** @brief ISOTP sender status. */
typedef enum
{
    ISOTP_SEND_STATUS_IDLE,
    ISOTP_SEND_STATUS_INPROGRESS,
    ISOTP_SEND_STATUS_ERROR,
} IsoTpSendStatusTypes;

/** @brief ISOTP receiver status. */
typedef enum
{
    ISOTP_RECEIVE_STATUS_IDLE,
    ISOTP_RECEIVE_STATUS_INPROGRESS,
    ISOTP_RECEIVE_STATUS_FULL,
} IsoTpReceiveStatusTypes;

/** @brief CAN frame definitions. */
#if defined(ISOTP_BYTE_ORDER_LITTLE_ENDIAN)
/**
 * @brief Single frame layout.
 *
 * @code{.text}
 * +-------------------------+-----+
 * | byte #0                 | ... |
 * +-------------------------+-----+
 * | nibble #0   | nibble #1 | ... |
 * +-------------+-----------+ ... +
 * | PCIType = 0 | SF_DL     | ... |
 * +-------------+-----------+-----+
 * @endcode
 */
typedef struct
{
    uint8_t reserve_1 : 4; /**< Reserved. */
    uint8_t type : 4;      /**< PCI type. */
    uint8_t reserve_2[7];  /**< Reserved. */
} IsoTpPciType;

/**
 * @brief Single frame layout.
 *
 * @code{.text}
 * +-------------------------+-----+
 * | byte #0                 | ... |
 * +-------------------------+-----+
 * | nibble #0   | nibble #1 | ... |
 * +-------------+-----------+ ... +
 * | PCIType = 0 | SF_DL     | ... |
 * +-------------+-----------+-----+
 * @endcode
 */
typedef struct
{
    uint8_t SF_DL : 4; /**< Single-frame data length. */
    uint8_t type : 4;  /**< PCI type. */
    uint8_t data[7];   /**< Payload bytes. */
} IsoTpSingleFrame;

/**
 * @brief First frame (short) layout.
 *
 * @code{.text}
 * +-------------------------+-----------------------+-----+
 * | byte #0                 | byte #1               | ... |
 * +-------------------------+-----------+-----------+-----+
 * | nibble #0   | nibble #1 | nibble #2 | nibble #3 | ... |
 * +-------------+-----------+-----------+-----------+-----+
 * | PCIType = 1 | FF_DL                             | ... |
 * +-------------+-----------+-----------------------+-----+
 * @endcode
 */
typedef struct
{
    uint8_t FF_DL_high : 4; /**< First-frame data length (high nibble). */
    uint8_t type : 4;       /**< PCI type. */
    uint8_t FF_DL_low;      /**< First-frame data length (low byte). */
    uint8_t data[6];        /**< Payload bytes. */
} IsoTpFirstFrameShort;

/**
 * @brief First frame (long) layout.
 *
 * @code{.text}
 * +-------------------------+-----------------------+---------+---------+---------+---------+
 * | byte #0                 | byte #1               | byte #2 | byte #3 | byte #4 | byte #5 |
 * +-------------------------+-----------+-----------+---------+---------+---------+---------+
 * | nibble #0   | nibble #1 | nibble #2 | nibble #3 | ...                                   |
 * +-------------+-----------+-----------+-----------+---------------------------------------+
 * | PCIType = 1 | unused=0  | escape sequence = 0   | FF_DL                                 |
 * +-------------+-----------+-----------------------+---------------------------------------+
 * @endcode
 */
ISOTP_PACKED_STRUCT({
    uint8_t set_to_zero_high : 4; /**< Must be zero. */
    uint8_t type : 4;             /**< PCI type. */
    uint8_t set_to_zero_low;      /**< Must be zero. */
    uint32_t FF_DL;               /**< First-frame data length. */
    uint8_t data[2];              /**< Payload bytes. */
} IsoTpFirstFrameLong);

/**
 * @brief Consecutive frame layout.
 *
 * @code{.text}
 * +-------------------------+-----+
 * | byte #0                 | ... |
 * +-------------------------+-----+
 * | nibble #0   | nibble #1 | ... |
 * +-------------+-----------+ ... +
 * | PCIType = 0 | SN        | ... |
 * +-------------+-----------+-----+
 * @endcode
 */
typedef struct
{
    uint8_t SN : 4;   /**< Sequence number. */
    uint8_t type : 4; /**< PCI type. */
    uint8_t data[7];  /**< Payload bytes. */
} IsoTpConsecutiveFrame;

/**
 * @brief Flow control frame layout.
 *
 * @code{.text}
 * +-------------------------+-----------------------+-----------------------+-----+
 * | byte #0                 | byte #1               | byte #2               | ... |
 * +-------------------------+-----------+-----------+-----------+-----------+-----+
 * | nibble #0   | nibble #1 | nibble #2 | nibble #3 | nibble #4 | nibble #5 | ... |
 * +-------------+-----------+-----------+-----------+-----------+-----------+-----+
 * | PCIType = 1 | FS        | BS                    | STmin                 | ... |
 * +-------------+-----------+-----------------------+-----------------------+-----+
 * @endcode
 */
typedef struct
{
    uint8_t FS : 4;     /**< Flow status. */
    uint8_t type : 4;   /**< PCI type. */
    uint8_t BS;         /**< Block size. */
    uint8_t STmin;      /**< Minimum separation time. */
    uint8_t reserve[5]; /**< Reserved. */
} IsoTpFlowControl;

#else

typedef struct
{
    uint8_t type : 4;      /**< PCI type. */
    uint8_t reserve_1 : 4; /**< Reserved. */
    uint8_t reserve_2[7];  /**< Reserved. */
} IsoTpPciType;

/**
 * @brief Single frame layout.
 *
 * @code{.text}
 * +-------------------------+-----+
 * | byte #0                 | ... |
 * +-------------------------+-----+
 * | nibble #0   | nibble #1 | ... |
 * +-------------+-----------+ ... +
 * | PCIType = 0 | SF_DL     | ... |
 * +-------------+-----------+-----+
 * @endcode
 */
typedef struct
{
    uint8_t type : 4;  /**< PCI type. */
    uint8_t SF_DL : 4; /**< Single-frame data length. */
    uint8_t data[7];   /**< Payload bytes. */
} IsoTpSingleFrame;

/**
 * @brief First frame (short) layout.
 *
 * @code{.text}
 * +-------------------------+-----------------------+-----+
 * | byte #0                 | byte #1               | ... |
 * +-------------------------+-----------+-----------+-----+
 * | nibble #0   | nibble #1 | nibble #2 | nibble #3 | ... |
 * +-------------+-----------+-----------+-----------+-----+
 * | PCIType = 1 | FF_DL                             | ... |
 * +-------------+-----------+-----------------------+-----+
 * @endcode
 */
typedef struct
{
    uint8_t FF_DL_high : 4; /**< First-frame data length (high nibble). */
    uint8_t type : 4;       /**< PCI type. */
    uint8_t FF_DL_low;      /**< First-frame data length (low byte). */
    uint8_t data[6];        /**< Payload bytes. */
} IsoTpFirstFrameShort;

/**
 * @brief First frame (long) layout.
 *
 * @code{.text}
 * +-------------------------+-----------------------+---------+---------+---------+---------+
 * | byte #0                 | byte #1               | byte #2 | byte #3 | byte #4 | byte #5 |
 * +-------------------------+-----------+-----------+---------+---------+---------+---------+
 * | nibble #0   | nibble #1 | nibble #2 | nibble #3 | ...                                   |
 * +-------------+-----------+-----------+-----------+---------------------------------------+
 * | PCIType = 1 | unused=0  | escape sequence = 0   | FF_DL                                 |
 * +-------------+-----------+-----------------------+---------------------------------------+
 * @endcode
 */
ISOTP_PACKED_STRUCT({
    uint8_t set_to_zero_high : 4; /**< Must be zero. */
    uint8_t type : 4;             /**< PCI type. */
    uint8_t set_to_zero_low;      /**< Must be zero. */
    uint32_t FF_DL;               /**< First-frame data length. */
    uint8_t data[2];              /**< Payload bytes. */
} IsoTpFirstFrameLong);

/**
 * @brief Consecutive frame layout.
 *
 * @code{.text}
 * +-------------------------+-----+
 * | byte #0                 | ... |
 * +-------------------------+-----+
 * | nibble #0   | nibble #1 | ... |
 * +-------------+-----------+ ... +
 * | PCIType = 0 | SN        | ... |
 * +-------------+-----------+-----+
 * @endcode
 */
typedef struct
{
    uint8_t type : 4; /**< PCI type. */
    uint8_t SN : 4;   /**< Sequence number. */
    uint8_t data[7];  /**< Payload bytes. */
} IsoTpConsecutiveFrame;

/**
 * @brief Flow control frame layout.
 *
 * @code{.text}
 * +-------------------------+-----------------------+-----------------------+-----+
 * | byte #0                 | byte #1               | byte #2               | ... |
 * +-------------------------+-----------+-----------+-----------+-----------+-----+
 * | nibble #0   | nibble #1 | nibble #2 | nibble #3 | nibble #4 | nibble #5 | ... |
 * +-------------+-----------+-----------+-----------+-----------+-----------+-----+
 * | PCIType = 1 | FS        | BS                    | STmin                 | ... |
 * +-------------+-----------+-----------------------+-----------------------+-----+
 * @endcode
 */
typedef struct
{
    uint8_t type : 4;   /**< PCI type. */
    uint8_t FS : 4;     /**< Flow status. */
    uint8_t BS;         /**< Block size. */
    uint8_t STmin;      /**< Minimum separation time. */
    uint8_t reserve[5]; /**< Reserved. */
} IsoTpFlowControl;

#endif

typedef struct
{
    uint8_t ptr[8];
} IsoTpDataArray;

typedef struct
{
    union
    {
        IsoTpPciType common;
        IsoTpSingleFrame single_frame;
        IsoTpFirstFrameShort first_frame_short;
        IsoTpFirstFrameLong first_frame_long;
        IsoTpConsecutiveFrame consecutive_frame;
        IsoTpFlowControl flow_control;
        IsoTpDataArray data_array;
    } as;
} IsoTpCanMessage;

/** @brief Protocol-specific defines. */

#ifdef ISO_TP_TRANSMIT_COMPLETE_CALLBACK
/**
 * @brief Function pointer type for transmission done callback.
 * @details Called when any transmission (single-frame or multi-frame) completes successfully.
 * @internal
 */
typedef void (*isotp_tx_done_cb)(void* link, uint32_t tx_size, void* user_arg);
#endif

#ifdef ISO_TP_RECEIVE_COMPLETE_CALLBACK
/**
 * @brief Function pointer type for receive done callback.
 * @details Called when a complete message (single-frame or multi-frame) is received successfully.
 * @internal
 */
typedef void (*isotp_rx_done_cb)(void* link, const uint8_t* data, uint32_t size, void* user_arg);
#endif

/**
 * @brief Protocol Control Information (PCI) types for identifying ISO-TP frames.
 * @internal
 */
typedef enum
{
    ISOTP_PCI_TYPE_SINGLE = 0x0,
    ISOTP_PCI_TYPE_FIRST_FRAME = 0x1,
    TSOTP_PCI_TYPE_CONSECUTIVE_FRAME = 0x2,
    ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME = 0x3,

    ISOTP_PCI_TYPE_CONSECUTIVE_FRAME = 0x2, /**< Typo fix; keep broken value for compatibility. */
} IsoTpProtocolControlInformation;

/**
 * @brief Protocol Control Information (PCI) flow control identifiers.
 * @internal
 */
typedef enum
{
    PCI_FLOW_STATUS_CONTINUE = 0x0,
    PCI_FLOW_STATUS_WAIT = 0x1,
    PCI_FLOW_STATUS_OVERFLOW = 0x2
} IsoTpFlowStatus;

/**
 * @brief Network layer result codes.
 * @internal
 */
#define ISOTP_PROTOCOL_RESULT_OK 0
#define ISOTP_PROTOCOL_RESULT_TIMEOUT_A -1
#define ISOTP_PROTOCOL_RESULT_TIMEOUT_BS -2
#define ISOTP_PROTOCOL_RESULT_TIMEOUT_CR -3
#define ISOTP_PROTOCOL_RESULT_WRONG_SN -4
#define ISOTP_PROTOCOL_RESULT_INVALID_FS -5
#define ISOTP_PROTOCOL_RESULT_UNEXP_PDU -6
#define ISOTP_PROTOCOL_RESULT_WFT_OVRN -7
#define ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW -8
#define ISOTP_PROTOCOL_RESULT_ERROR -9

/** @} */

/** @name Type definitions */
/** @{ */
/** @} */

/** @name Global variables */
/** @{ */
/** @} */

/** @name Public function declarations */
/** @{ */
/** @} */

/** @} */

#endif /* ISOTPC_USER_DEFINITIONS_H */