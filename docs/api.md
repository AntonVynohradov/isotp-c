# API Reference

This chapter documents every public function, type and macro of ISOTP-C. Include the
umbrella header:

```c
#include "isotp.h"
```

| Header | Contents |
| --- | --- |
| `isotp.h` | All headers; library version |
| `isotp_config.h` | Build-time configuration |
| `isotp_defs.h` | Frame types, flow status, layout limits, return and N_Result codes, big-endian helpers |
| `isotp_protocol.h` | Frame codec: decode, encode SF/FF/CF/FC, padding, STmin |
| `isotp_link.h` | Transport layer of one connection |

## Version — `isotp.h`

| Macro | Meaning |
| --- | --- |
| `ISOTP_VERSION_MAJOR`, `ISOTP_VERSION_MINOR`, `ISOTP_VERSION_PATCH` | Version numbers from the Git tag (CMake) or the fallbacks in `isotp.h` |
| `ISOTP_VERSION` | Version string `"MAJOR.MINOR.PATCH"` |

## Configuration — `isotp_config.h`

| Macro | Default | Meaning |
| --- | --- | --- |
| `ISOTP_TX_BUFFER_SIZE` | `4095U` | Largest message a link sends |
| `ISOTP_RX_BUFFER_SIZE` | `4095U` | Largest message a link receives |
| `ISOTP_DEFAULT_BLOCK_SIZE` | `8U` | Default BS requested by the receiver |
| `ISOTP_DEFAULT_ST_MIN_US` | `0U` | Default STmin requested by the receiver |
| `ISOTP_DEFAULT_N_BS_TIMEOUT_US` | `1000000U` | Default N_Bs |
| `ISOTP_DEFAULT_N_CR_TIMEOUT_US` | `1000000U` | Default N_Cr |
| `ISOTP_DEFAULT_WFT_MAX` | `10U` | Default N_WFTmax |

Ranges and memory: see [Configuration](configuration.md).

## Definitions — `isotp_defs.h`

| Macro | Value | Meaning |
| --- | --- | --- |
| `ISOTP_OK` | `0` | Success |
| `ISOTP_ERROR` | `-1` | Invalid argument, malformed frame or insufficient capacity |
| `ISOTP_BUSY` | `-2` | Try again later: transmission in progress or CAN driver busy |
| `ISOTP_PCI_SINGLE_FRAME` … `ISOTP_PCI_FLOW_CONTROL` | `0x0` … `0x3` | Frame types |
| `ISOTP_FS_CONTINUE`, `ISOTP_FS_WAIT`, `ISOTP_FS_OVERFLOW` | `0x0`, `0x1`, `0x2` | Flow status |
| `ISOTP_CAN_DL` | `8U` | Data bytes of a classical CAN frame |
| `ISOTP_SF_MAX_DL` | `7U` | Largest Single Frame payload |
| `ISOTP_FF_DL_12BIT_MAX` | `4095U` | Largest 12-bit FF_DL |
| `ISOTP_FF_SHORT_DATA`, `ISOTP_FF_LONG_DATA` | `6U`, `2U` | Payload bytes in a First Frame (12-bit / 32-bit FF_DL) |
| `ISOTP_CF_MAX_DATA` | `7U` | Payload bytes in a full Consecutive Frame |
| `ISOTP_FC_DL` | `3U` | Flow Control frame length without padding |
| `ISOTP_SN_MASK`, `ISOTP_FIRST_SN` | `0x0FU`, `1U` | Sequence numbers |
| `ISOTP_ST_MIN_MAX_US` | `127000U` | Largest STmin |
| `ISOTP_N_OK` … `ISOTP_N_ERROR` | `0` … `-9` | N_Result codes, see [Protocol](protocol.md#timers-and-n_result) |

```c
static inline uint32_t isotp_get_u32_be(const uint8_t* data)
```

Reads a big-endian 32-bit value from four bytes.

```c
static inline void isotp_put_u32_be(uint8_t* data, uint32_t value)
```

Writes a 32-bit value as four big-endian bytes.

## Codec — `isotp_protocol.h`

```c
struct isotp_pdu
{
    uint8_t type;
    uint32_t message_length;
    uint8_t data_offset;
    uint8_t data_length;
    uint8_t sequence_number;
    uint8_t flow_status;
    uint8_t block_size;
    uint8_t st_min;
};
```

One decoded frame. Only the members of the decoded type are meaningful; the others are
zero. The payload is not copied: it is `frame[data_offset]` … `frame[data_offset +
data_length - 1]` of the decoded frame.

| Member | SF | FF | CF | FC |
| --- | --- | --- | --- | --- |
| `type` | `ISOTP_PCI_SINGLE_FRAME` | `ISOTP_PCI_FIRST_FRAME` | `ISOTP_PCI_CONSECUTIVE_FRAME` | `ISOTP_PCI_FLOW_CONTROL` |
| `message_length` | SF_DL | FF_DL | — | — |
| `data_offset`, `data_length` | 1, SF_DL | 2, 6 (or 6, 2) | 1, frame length − 1 | — |
| `sequence_number` | — | — | SN | — |
| `flow_status`, `block_size`, `st_min` | — | — | — | FS, BS, raw STmin |

### isotp_decode

```c
int isotp_decode(const uint8_t* frame, size_t length, struct isotp_pdu* pdu);
```

Validates one received CAN frame and describes it. Rejects the frames ISO 15765-2 tells a
receiver to ignore (see [Receiver behavior](protocol.md#receiver-behavior)). The flow
status of a Flow Control frame is not range-checked: an unknown value is an N_INVALID_FS
condition for the sender.

| Parameter | Description |
| --- | --- |
| `frame` | CAN frame data |
| `length` | CAN data length, 1..8 |
| `pdu` | Receives the description |

| Return | Meaning |
| --- | --- |
| `ISOTP_OK` | `pdu` describes a valid frame |
| `ISOTP_ERROR` | `NULL` argument, invalid length or malformed frame |

### isotp_encode_single

```c
int isotp_encode_single(uint8_t* frame, size_t capacity, const uint8_t* payload, uint32_t length);
```

Encodes a Single Frame.

| Parameter | Description |
| --- | --- |
| `frame` | Output buffer |
| `capacity` | Size of `frame`, at least `length` + 1 |
| `payload` | Message bytes |
| `length` | Message length, 1..7 |

| Return | Meaning |
| --- | --- |
| `length` + 1 | Frame length |
| `ISOTP_ERROR` | `NULL` argument, length out of range or buffer too small |

### isotp_encode_first

```c
int isotp_encode_first(uint8_t* frame, size_t capacity, const uint8_t* payload,
                       uint32_t message_length);
```

Encodes a First Frame and its first payload bytes: a 12-bit FF_DL and 6 bytes for
messages up to 4095 bytes, the escape sequence with a 32-bit FF_DL and 2 bytes above.

| Parameter | Description |
| --- | --- |
| `frame` | Output buffer |
| `capacity` | Size of `frame`, at least 8 |
| `payload` | Start of the message; 6 (or 2) bytes are read |
| `message_length` | Length of the whole message, at least 8 |

| Return | Meaning |
| --- | --- |
| `8` | Frame length |
| `ISOTP_ERROR` | `NULL` argument, message shorter than 8 bytes or buffer too small |

### isotp_encode_consecutive

```c
int isotp_encode_consecutive(uint8_t* frame, size_t capacity, uint8_t sequence_number,
                             const uint8_t* data, uint32_t length);
```

Encodes a Consecutive Frame.

| Parameter | Description |
| --- | --- |
| `frame` | Output buffer |
| `capacity` | Size of `frame`, at least `length` + 1 |
| `sequence_number` | SN, 0..15 |
| `data` | Segment bytes |
| `length` | Segment length, 1..7 |

| Return | Meaning |
| --- | --- |
| `length` + 1 | Frame length |
| `ISOTP_ERROR` | `NULL` argument, value out of range or buffer too small |

### isotp_encode_flow_control

```c
int isotp_encode_flow_control(uint8_t* frame, size_t capacity, uint8_t flow_status,
                              uint8_t block_size, uint8_t st_min);
```

Encodes a Flow Control frame.

| Parameter | Description |
| --- | --- |
| `frame` | Output buffer |
| `capacity` | Size of `frame`, at least 3 |
| `flow_status` | `ISOTP_FS_CONTINUE`, `ISOTP_FS_WAIT` or `ISOTP_FS_OVERFLOW` |
| `block_size` | BS, 0 = no further Flow Control |
| `st_min` | Raw STmin byte, see `isotp_st_min_encode()` |

| Return | Meaning |
| --- | --- |
| `3` | Frame length |
| `ISOTP_ERROR` | `NULL` argument, unknown flow status or buffer too small |

### isotp_frame_pad

```c
int isotp_frame_pad(uint8_t* frame, size_t capacity, size_t length, uint8_t value);
```

Fills a frame with a padding byte up to 8 data bytes.

| Parameter | Description |
| --- | --- |
| `frame` | Frame buffer |
| `capacity` | Size of `frame`, at least 8 |
| `length` | Current frame length, 1..8 |
| `value` | Padding byte |

| Return | Meaning |
| --- | --- |
| `8` | New frame length |
| `ISOTP_ERROR` | `NULL` argument, length out of range or buffer too small |

### isotp_st_min_encode

```c
uint8_t isotp_st_min_encode(uint32_t us);
```

Encodes a separation time for a Flow Control frame, rounded up: 0 µs gives `0x00`,
1..900 µs give `0xF1`..`0xF9`, 901..127 000 µs give `0x01`..`0x7F`, larger values `0x7F`.

| Parameter | Description |
| --- | --- |
| `us` | Separation time in microseconds |

| Return | Meaning |
| --- | --- |
| `0x00`..`0x7F`, `0xF1`..`0xF9` | Raw STmin byte |

### isotp_st_min_decode

```c
uint32_t isotp_st_min_decode(uint8_t st_min);
```

Decodes a received STmin byte. Reserved values give 127 000 µs, as ISO 15765-2 requires.

| Parameter | Description |
| --- | --- |
| `st_min` | Raw STmin byte |

| Return | Meaning |
| --- | --- |
| 0..127 000 | Separation time in microseconds |

## Link — `isotp_link.h`

```c
typedef int (*isotp_can_write_fn)(void* context, uint32_t can_id, const uint8_t* data,
                                  size_t length);
```

Writes one CAN frame. Returns `0` when the frame is queued, `ISOTP_BUSY` when the driver
has no free mailbox (a Consecutive Frame is retried on the next poll), any other value on
a driver error. `data` is valid only during the call.

```c
typedef void (*isotp_message_handler_t)(const uint8_t* data, uint32_t length, void* context);
```

Receives one complete message. `data` is valid only during the call. The handler may call
`isotp_link_send()` on the same link.

```c
typedef void (*isotp_result_fn)(int n_result, void* context);
```

Receives the N_Result of a finished transmission (`on_tx_done`) or of a failed reception
(`on_rx_error`).

```c
struct isotp_link_config
{
    uint32_t tx_id;
    isotp_can_write_fn write;
    void* write_context;
    isotp_message_handler_t on_message;
    isotp_result_fn on_tx_done;
    isotp_result_fn on_rx_error;
    void* app_context;
    uint8_t block_size;
    uint32_t st_min_us;
    uint32_t n_bs_timeout_us;
    uint32_t n_cr_timeout_us;
    uint8_t wft_max;
    bool padding;
    uint8_t padding_value;
};
```

| Member | Meaning |
| --- | --- |
| `tx_id` | CAN ID of every frame this link sends |
| `write`, `write_context` | Frame writer (required) and the context passed to it |
| `on_message` | Complete received message; `NULL` discards it |
| `on_tx_done` | End of every transmission started by `isotp_link_send()`; may be `NULL` |
| `on_rx_error` | Failed or interrupted reception; may be `NULL` |
| `app_context` | Context passed to the three callbacks above |
| `block_size`, `st_min_us` | BS and STmin requested from the sender (STmin 0..127 000 µs) |
| `n_bs_timeout_us`, `n_cr_timeout_us` | N_Bs and N_Cr, 1..2^31−1 µs |
| `wft_max` | N_WFTmax: Wait frames accepted in a row |
| `padding`, `padding_value` | Pad every sent frame to 8 bytes with this byte |

`struct isotp_link` holds the state of one link: a copy of the configuration, both
message buffers, both state machines and the invalid-frame counter. Declare it with
static storage (it holds the buffers) and treat its members as private.

### isotp_link_config_default

```c
void isotp_link_config_default(struct isotp_link_config* config);
```

Fills a configuration with CAN ID 0, no writer, no callbacks, no padding (padding byte
`0xCC`) and the `ISOTP_DEFAULT_*` flow-control and timing values. Nothing happens for
`NULL`.

### isotp_link_init

```c
int isotp_link_init(struct isotp_link* link, const struct isotp_link_config* config);
```

Copies the configuration and clears both directions. Can be called again to reset a link.

| Parameter | Description |
| --- | --- |
| `link` | Link state to initialize |
| `config` | Configuration copied into the link |

| Return | Meaning |
| --- | --- |
| `ISOTP_OK` | Link ready |
| `ISOTP_ERROR` | `NULL` argument, no writer, STmin above 127 000 µs, or a timeout of 0 or above 2^31−1 µs |

### isotp_link_receive

```c
void isotp_link_receive(struct isotp_link* link, const uint8_t* data, size_t length,
                        uint32_t now_us);
```

Processes one received CAN frame of the peer. Delivers complete messages to
`on_message()`, answers First Frames and finished blocks with Flow Control, hands Flow
Control frames to the transmitter, ignores and counts malformed frames. See
[The receiver](architecture.md#the-receiver).

| Parameter | Description |
| --- | --- |
| `link` | Link state; nothing happens for `NULL` |
| `data` | CAN frame data |
| `length` | CAN data length, 1..8 |
| `now_us` | Current time in microseconds (free-running, wraps at 2^32) |

### isotp_link_send

```c
int isotp_link_send(struct isotp_link* link, const uint8_t* payload, uint32_t length,
                    uint32_t now_us);
```

Starts sending one message. Up to 7 bytes are sent at once as a Single Frame and reported
to `on_tx_done()` before the call returns. A longer message is copied into the link, its
First Frame is written, and the rest is sent from `isotp_link_poll()`.

| Parameter | Description |
| --- | --- |
| `link` | Link state |
| `payload` | Message bytes; may be reused when the call returns |
| `length` | Message length, 1..`ISOTP_TX_BUFFER_SIZE` |
| `now_us` | Current time in microseconds |

| Return | Meaning |
| --- | --- |
| `ISOTP_OK` | Message accepted |
| `ISOTP_BUSY` | A segmented transmission is in progress, or the writer is busy |
| `ISOTP_ERROR` | `NULL` argument, length 0 or message too long |
| other | Error returned by the writer; nothing was started |

### isotp_link_send_functional

```c
int isotp_link_send_functional(const struct isotp_link* link, uint32_t can_id,
                               const uint8_t* payload, uint32_t length);
```

Sends a Single Frame with a functional CAN ID. Independent of the link's transmission
state; does not call `on_tx_done()`.

| Parameter | Description |
| --- | --- |
| `link` | Link state (its writer and padding are used) |
| `can_id` | Functional CAN ID |
| `payload` | Message bytes |
| `length` | Message length, 1..7 |

| Return | Meaning |
| --- | --- |
| `ISOTP_OK` | Frame written |
| `ISOTP_ERROR` | `NULL` argument or length out of range |
| other | Error returned by the writer |

### isotp_link_poll

```c
void isotp_link_poll(struct isotp_link* link, uint32_t now_us);
```

Sends the next Consecutive Frame when the separation time has passed (at most one per
call), and ends a transmission or reception whose timer N_Bs or N_Cr has expired.

| Parameter | Description |
| --- | --- |
| `link` | Link state; nothing happens for `NULL` |
| `now_us` | Current time in microseconds |

### Diagnostics

```c
bool isotp_link_tx_busy(const struct isotp_link* link);
```

```c
int isotp_link_tx_result(const struct isotp_link* link);
```

```c
int isotp_link_rx_result(const struct isotp_link* link);
```

```c
uint32_t isotp_link_invalid_frames(const struct isotp_link* link);
```

| Function | Returns | For `NULL` |
| --- | --- | --- |
| `isotp_link_tx_busy()` | `true` while a segmented transmission is in progress | `false` |
| `isotp_link_tx_result()` | N_Result of the last finished transmission | `ISOTP_N_ERROR` |
| `isotp_link_rx_result()` | N_Result of the last finished or failed reception | `ISOTP_N_ERROR` |
| `isotp_link_invalid_frames()` | Malformed frames ignored since `isotp_link_init()` | `0` |

## Return codes summary

| Value | Meaning |
| --- | --- |
| `ISOTP_OK` (`0`) | Success |
| `ISOTP_ERROR` (`-1`) | Invalid argument, malformed frame or insufficient capacity |
| `ISOTP_BUSY` (`-2`) | Try again later |
| `1`..`8` | Frame length (encoders, `isotp_frame_pad()`) |
| other | Passed through from your write function |
