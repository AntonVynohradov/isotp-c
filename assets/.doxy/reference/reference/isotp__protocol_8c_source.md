

# File isotp\_protocol.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**isotp\_protocol.c**](isotp__protocol_8c.md)

[Go to the documentation of this file](isotp__protocol_8c.md)


```C++

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <stdbool.h>
#include <string.h>

#include "isotp_protocol.h"

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define ISOTP_PCI_TYPE_SHIFT 4U   /* frame type is the high nibble of byte 0 */
#define ISOTP_LOW_NIBBLE 0x0FU    /* SF_DL, FF_DL high bits, SN and FS */
#define ISOTP_FF_SHORT_OFFSET 2U  /* first payload byte with the 12-bit FF_DL */
#define ISOTP_FF_ESCAPE_OFFSET 6U /* first payload byte with the 32-bit FF_DL */
#define ISOTP_ST_MIN_MS_MAX 0x7FU /* 0x00..0x7F: milliseconds */
#define ISOTP_ST_MIN_US_FIRST 0xF1U
#define ISOTP_ST_MIN_US_LAST 0xF9U
#define ISOTP_ST_MIN_US_BASE 0xF0U
#define ISOTP_US_PER_MS 1000U
#define ISOTP_US_PER_STEP 100U
#define ISOTP_ST_MIN_US_STEPS_MAX 900U

/* ==============================================================================
 * PRIVATE FUNCTION DECLARATIONS (static)
 * =============================================================================*/

static void isotp_pdu_clear(struct isotp_pdu* pdu);
static bool isotp_decode_single(const uint8_t* frame, size_t length, struct isotp_pdu* pdu);
static bool isotp_decode_first(const uint8_t* frame, size_t length, struct isotp_pdu* pdu);
static bool isotp_decode_consecutive(const uint8_t* frame, size_t length, struct isotp_pdu* pdu);
static bool isotp_decode_flow_control(const uint8_t* frame, size_t length, struct isotp_pdu* pdu);

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

static void isotp_pdu_clear(struct isotp_pdu* pdu)
{
    pdu->type = 0U;
    pdu->message_length = 0U;
    pdu->data_offset = 0U;
    pdu->data_length = 0U;
    pdu->sequence_number = 0U;
    pdu->flow_status = 0U;
    pdu->block_size = 0U;
    pdu->st_min = 0U;
}

static bool isotp_decode_single(const uint8_t* frame, size_t length, struct isotp_pdu* pdu)
{
    uint8_t sf_dl = (uint8_t) (frame[0] & ISOTP_LOW_NIBBLE);
    bool valid = (sf_dl > 0U) && ((size_t) sf_dl < length);

    if (valid)
    {
        pdu->message_length = sf_dl;
        pdu->data_offset = 1U;
        pdu->data_length = sf_dl;
    }
    return valid;
}

static bool isotp_decode_first(const uint8_t* frame, size_t length, struct isotp_pdu* pdu)
{
    bool valid = (length == (size_t) ISOTP_CAN_DL);

    if (valid)
    {
        uint32_t ff_dl = (((uint32_t) frame[0] & ISOTP_LOW_NIBBLE) << 8U) | (uint32_t) frame[1];

        if (ff_dl == 0U)
        {
            /* Escape sequence: 32-bit FF_DL in bytes 2..5, only for long messages */
            ff_dl = isotp_get_u32_be(&frame[ISOTP_FF_SHORT_OFFSET]);
            valid = (ff_dl > ISOTP_FF_DL_12BIT_MAX);
            pdu->data_offset = ISOTP_FF_ESCAPE_OFFSET;
            pdu->data_length = ISOTP_FF_LONG_DATA;
        }
        else
        {
            /* A message that fits a Single Frame must not be segmented */
            valid = (ff_dl > ISOTP_SF_MAX_DL);
            pdu->data_offset = ISOTP_FF_SHORT_OFFSET;
            pdu->data_length = ISOTP_FF_SHORT_DATA;
        }
        pdu->message_length = ff_dl;
    }
    return valid;
}

static bool isotp_decode_consecutive(const uint8_t* frame, size_t length, struct isotp_pdu* pdu)
{
    bool valid = (length > 1U);

    if (valid)
    {
        pdu->sequence_number = (uint8_t) (frame[0] & ISOTP_LOW_NIBBLE);
        pdu->data_offset = 1U;
        pdu->data_length = (uint8_t) (length - 1U);
    }
    return valid;
}

static bool isotp_decode_flow_control(const uint8_t* frame, size_t length, struct isotp_pdu* pdu)
{
    bool valid = (length >= (size_t) ISOTP_FC_DL);

    if (valid)
    {
        pdu->flow_status = (uint8_t) (frame[0] & ISOTP_LOW_NIBBLE);
        pdu->block_size = frame[1];
        pdu->st_min = frame[2];
    }
    return valid;
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

int isotp_decode(const uint8_t* frame, size_t length, struct isotp_pdu* pdu)
{
    int result = ISOTP_ERROR;

    if ((frame != NULL) && (pdu != NULL) && (length > 0U) && (length <= (size_t) ISOTP_CAN_DL))
    {
        uint8_t type = (uint8_t) (frame[0] >> ISOTP_PCI_TYPE_SHIFT);
        bool valid = false;

        isotp_pdu_clear(pdu);
        pdu->type = type;

        switch (type)
        {
            case ISOTP_PCI_SINGLE_FRAME:
                valid = isotp_decode_single(frame, length, pdu);
                break;
            case ISOTP_PCI_FIRST_FRAME:
                valid = isotp_decode_first(frame, length, pdu);
                break;
            case ISOTP_PCI_CONSECUTIVE_FRAME:
                valid = isotp_decode_consecutive(frame, length, pdu);
                break;
            case ISOTP_PCI_FLOW_CONTROL:
                valid = isotp_decode_flow_control(frame, length, pdu);
                break;
            default:
                /* Reserved frame types are ignored by the receiver */
                break;
        }

        if (valid)
        {
            result = ISOTP_OK;
        }
    }
    return result;
}

int isotp_encode_single(uint8_t* frame, size_t capacity, const uint8_t* payload, uint32_t length)
{
    int result = ISOTP_ERROR;

    if ((frame != NULL) && (payload != NULL) && (length > 0U) && (length <= ISOTP_SF_MAX_DL)
        && (capacity > (size_t) length))
    {
        frame[0] = (uint8_t) ((ISOTP_PCI_SINGLE_FRAME << ISOTP_PCI_TYPE_SHIFT) | length);
        (void) memcpy(&frame[1], payload, (size_t) length);
        result = (int) length + 1;
    }
    return result;
}

int isotp_encode_first(uint8_t* frame, size_t capacity, const uint8_t* payload,
                       uint32_t message_length)
{
    int result = ISOTP_ERROR;

    if ((frame != NULL) && (payload != NULL) && (message_length > ISOTP_SF_MAX_DL)
        && (capacity >= (size_t) ISOTP_CAN_DL))
    {
        if (message_length <= ISOTP_FF_DL_12BIT_MAX)
        {
            frame[0] = (uint8_t) ((ISOTP_PCI_FIRST_FRAME << ISOTP_PCI_TYPE_SHIFT)
                                  | (message_length >> 8U));
            frame[1] = (uint8_t) (message_length & 0xFFU);
            (void) memcpy(&frame[ISOTP_FF_SHORT_OFFSET], payload, (size_t) ISOTP_FF_SHORT_DATA);
        }
        else
        {
            frame[0] = (uint8_t) (ISOTP_PCI_FIRST_FRAME << ISOTP_PCI_TYPE_SHIFT);
            frame[1] = 0U;
            isotp_put_u32_be(&frame[ISOTP_FF_SHORT_OFFSET], message_length);
            (void) memcpy(&frame[ISOTP_FF_ESCAPE_OFFSET], payload, (size_t) ISOTP_FF_LONG_DATA);
        }
        result = (int) ISOTP_CAN_DL;
    }
    return result;
}

int isotp_encode_consecutive(uint8_t* frame, size_t capacity, uint8_t sequence_number,
                             const uint8_t* data, uint32_t length)
{
    int result = ISOTP_ERROR;

    if ((frame != NULL) && (data != NULL) && (sequence_number <= ISOTP_SN_MASK) && (length > 0U)
        && (length <= ISOTP_CF_MAX_DATA) && (capacity > (size_t) length))
    {
        frame[0] = (uint8_t) ((ISOTP_PCI_CONSECUTIVE_FRAME << ISOTP_PCI_TYPE_SHIFT)
                              | (uint32_t) sequence_number);
        (void) memcpy(&frame[1], data, (size_t) length);
        result = (int) length + 1;
    }
    return result;
}

int isotp_encode_flow_control(uint8_t* frame, size_t capacity, uint8_t flow_status,
                              uint8_t block_size, uint8_t st_min)
{
    int result = ISOTP_ERROR;

    if ((frame != NULL) && (flow_status <= ISOTP_FS_OVERFLOW) && (capacity >= (size_t) ISOTP_FC_DL))
    {
        frame[0] =
            (uint8_t) ((ISOTP_PCI_FLOW_CONTROL << ISOTP_PCI_TYPE_SHIFT) | (uint32_t) flow_status);
        frame[1] = block_size;
        frame[2] = st_min;
        result = (int) ISOTP_FC_DL;
    }
    return result;
}

int isotp_frame_pad(uint8_t* frame, size_t capacity, size_t length, uint8_t value)
{
    int result = ISOTP_ERROR;

    if ((frame != NULL) && (capacity >= (size_t) ISOTP_CAN_DL) && (length > 0U)
        && (length <= (size_t) ISOTP_CAN_DL))
    {
        (void) memset(&frame[length], (int) value, (size_t) ISOTP_CAN_DL - length);
        result = (int) ISOTP_CAN_DL;
    }
    return result;
}

uint8_t isotp_st_min_encode(uint32_t us)
{
    uint8_t result;

    if (us == 0U)
    {
        result = 0U;
    }
    else if (us <= ISOTP_ST_MIN_US_STEPS_MAX)
    {
        /* 100 us steps, rounded up */
        result = (uint8_t) (ISOTP_ST_MIN_US_BASE
                            + ((us + (ISOTP_US_PER_STEP - 1U)) / ISOTP_US_PER_STEP));
    }
    else if (us < ISOTP_ST_MIN_MAX_US)
    {
        /* 1 ms steps, rounded up */
        result = (uint8_t) ((us + (ISOTP_US_PER_MS - 1U)) / ISOTP_US_PER_MS);
    }
    else
    {
        result = ISOTP_ST_MIN_MS_MAX;
    }
    return result;
}

uint32_t isotp_st_min_decode(uint8_t st_min)
{
    uint32_t result;

    if (st_min <= ISOTP_ST_MIN_MS_MAX)
    {
        result = (uint32_t) st_min * ISOTP_US_PER_MS;
    }
    else if ((st_min >= ISOTP_ST_MIN_US_FIRST) && (st_min <= ISOTP_ST_MIN_US_LAST))
    {
        result = ((uint32_t) st_min - ISOTP_ST_MIN_US_BASE) * ISOTP_US_PER_STEP;
    }
    else
    {
        /* Reserved values: ISO 15765-2 requires the longest time, 127 ms */
        result = ISOTP_ST_MIN_MAX_US;
    }
    return result;
}
```


