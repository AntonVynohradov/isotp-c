/**
 * @file isotp_protocol.h
 * @brief ISO-TP frame codec: decode one CAN frame, encode SF/FF/CF/FC, STmin.
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
 * @defgroup isotp_protocol Codec
 * @ingroup isotp
 * @brief Pure functions that translate between CAN frames and ISO-TP PDUs.
 *
 * @details The codec holds no state. @ref isotp_decode validates one received
 *          CAN frame and describes it in a @ref isotp_pdu; the encoders write
 *          one frame into a caller buffer and return its length. Frames are
 *          classical CAN (up to 8 data bytes) with normal addressing. The
 *          encoders produce the shortest frame; @ref isotp_frame_pad fills it
 *          up to 8 bytes when the bus requires padding.
 *
 * @par Public interface includes:
 * @li Decoding: @ref isotp_decode.
 * @li Encoding: @ref isotp_encode_single, @ref isotp_encode_first,
 *     @ref isotp_encode_consecutive, @ref isotp_encode_flow_control,
 *     @ref isotp_frame_pad.
 * @li Separation time: @ref isotp_st_min_encode, @ref isotp_st_min_decode.
 * @{
 */

#ifndef ISOTP_PROTOCOL_H
#define ISOTP_PROTOCOL_H

/** @name Includes */
/** @{ */

#include <stddef.h>
#include <stdint.h>

#include "isotp_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @} */

/** @name Type definitions */
/** @{ */

/**
 * @brief One decoded ISO-TP frame (protocol data unit).
 * @details Only the members of the decoded frame type are meaningful; the
 *          others are zero. The payload is not copied: it is
 *          `frame[data_offset]` .. `frame[data_offset + data_length - 1]` of the
 *          frame passed to @ref isotp_decode.
 */
struct isotp_pdu
{
    uint8_t type;            /**< ISOTP_PCI_* frame type. */
    uint32_t message_length; /**< SF_DL (SF) or FF_DL (FF): length of the whole message. */
    uint8_t data_offset;     /**< Index of the first payload byte in the frame. */
    uint8_t data_length;     /**< Payload bytes in this frame (SF, FF, CF). */
    uint8_t sequence_number; /**< SN (CF). */
    uint8_t flow_status;     /**< FS (FC), not range-checked: see @ref isotp_decode. */
    uint8_t block_size;      /**< BS (FC). */
    uint8_t st_min;          /**< Raw STmin byte (FC); convert with @ref isotp_st_min_decode. */
};

/** @} */

/** @name Public function declarations */
/** @{ */

/**
 * @brief Validate one received CAN frame and describe it.
 * @details Rejects frames that ISO 15765-2 tells a receiver to ignore: unknown
 *          frame type, SF_DL of 0 or longer than the frame, a First Frame that
 *          is not 8 bytes long or announces a message that would fit a Single
 *          Frame (or, with the 32-bit FF_DL, fits the 12-bit field), a
 *          Consecutive Frame without data, a Flow Control frame shorter than
 *          3 bytes. The flow status is not checked here: an unknown value is an
 *          N_INVALID_FS condition the sender has to report.
 * @param frame CAN frame data.
 * @param length CAN data length, 1..8.
 * @param pdu Receives the description.
 * @retval ISOTP_OK @p pdu describes a valid frame.
 * @retval ISOTP_ERROR NULL argument, invalid length or malformed frame.
 */
int isotp_decode(const uint8_t* frame, size_t length, struct isotp_pdu* pdu);

/**
 * @brief Encode a Single Frame.
 * @param frame Output buffer.
 * @param capacity Size of @p frame, at least @p length + 1.
 * @param payload Message bytes.
 * @param length Message length, 1..7.
 * @return Frame length (@p length + 1), or ISOTP_ERROR.
 */
int isotp_encode_single(uint8_t* frame, size_t capacity, const uint8_t* payload, uint32_t length);

/**
 * @brief Encode a First Frame and its first payload bytes.
 * @details Messages up to 4095 bytes use the 12-bit FF_DL and carry 6 payload
 *          bytes; longer messages use the escape sequence with a 32-bit FF_DL
 *          and carry 2 payload bytes.
 * @param frame Output buffer.
 * @param capacity Size of @p frame, at least 8.
 * @param payload Start of the message; at least 6 (or 2) bytes are read.
 * @param message_length Length of the whole message, at least 8.
 * @return Frame length (8), or ISOTP_ERROR.
 */
int isotp_encode_first(uint8_t* frame, size_t capacity, const uint8_t* payload,
                       uint32_t message_length);

/**
 * @brief Encode a Consecutive Frame.
 * @param frame Output buffer.
 * @param capacity Size of @p frame, at least @p length + 1.
 * @param sequence_number SN, 0..15.
 * @param data Segment bytes.
 * @param length Segment length, 1..7.
 * @return Frame length (@p length + 1), or ISOTP_ERROR.
 */
int isotp_encode_consecutive(uint8_t* frame, size_t capacity, uint8_t sequence_number,
                             const uint8_t* data, uint32_t length);

/**
 * @brief Encode a Flow Control frame.
 * @param frame Output buffer.
 * @param capacity Size of @p frame, at least 3.
 * @param flow_status ISOTP_FS_CONTINUE, ISOTP_FS_WAIT or ISOTP_FS_OVERFLOW.
 * @param block_size BS, 0 = no further Flow Control.
 * @param st_min Raw STmin byte, see @ref isotp_st_min_encode.
 * @return Frame length (3), or ISOTP_ERROR.
 */
int isotp_encode_flow_control(uint8_t* frame, size_t capacity, uint8_t flow_status,
                              uint8_t block_size, uint8_t st_min);

/**
 * @brief Fill a frame with a padding byte up to 8 data bytes.
 * @param frame Frame buffer.
 * @param capacity Size of @p frame, at least 8.
 * @param length Current frame length, 1..8.
 * @param value Padding byte.
 * @return New frame length (8), or ISOTP_ERROR.
 */
int isotp_frame_pad(uint8_t* frame, size_t capacity, size_t length, uint8_t value);

/**
 * @brief Encode a separation time for a Flow Control frame.
 * @details Rounds up, so the sender never sends faster than requested:
 *          0 us gives 0x00, 1..900 us give 0xF1..0xF9 (100 us steps),
 *          901..127000 us give 0x01..0x7F (1 ms steps), larger values 0x7F.
 * @param us Separation time in microseconds.
 * @return Raw STmin byte.
 */
uint8_t isotp_st_min_encode(uint32_t us);

/**
 * @brief Decode a received STmin byte.
 * @details 0x00..0x7F are milliseconds, 0xF1..0xF9 are 100..900 us. ISO 15765-2
 *          tells the sender to treat the reserved values as 0x7F (127 ms).
 * @param st_min Raw STmin byte.
 * @return Separation time in microseconds.
 */
uint32_t isotp_st_min_decode(uint8_t st_min);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* ISOTP_PROTOCOL_H */

/** @} */
