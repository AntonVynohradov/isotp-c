# Using the Library

A link is used the same way in an ECU (diagnostic server) and in a tester (diagnostic
client): the ECU waits for requests and answers them, the tester sends a request and
waits for the answer. This chapter shows both, how to use the codec on its own, what
every function returns, and the mistakes that are easy to make.

## Setting up a link

Start from the defaults, set the CAN ID this node sends with and the callbacks, and
initialize the link. The link keeps a copy of the configuration.

```c
static struct isotp_link link;

static int can_write(void* context, uint32_t can_id, const uint8_t* data, size_t length)
{
    (void) context;
    return board_can_write(can_id, data, length);
}

static void on_message(const uint8_t* data, uint32_t length, void* context)
{
    (void) data;
    (void) length;
    (void) context;
}

int setup(void)
{
    struct isotp_link_config config;

    isotp_link_config_default(&config);
    config.tx_id = 0x7E8U;          /* this ECU answers on 0x7E8 */
    config.write = can_write;
    config.on_message = on_message;
    config.block_size = 8U;         /* ask the tester for Flow Control every 8 frames */
    config.st_min_us = 1000U;       /* and 1 ms between its Consecutive Frames */
    config.padding = true;          /* most vehicles expect 8-byte frames */
    return isotp_link_init(&link, &config);
}
```

The link does not filter CAN IDs. Give it only the frames of its peer: in an ECU the
physical request ID (0x7E0 above) and, if used, the functional ID (0x7DF); in a tester the
response ID of the ECU.

## Writing an ECU

An ECU calls `isotp_link_receive()` for every frame of its request IDs and
`isotp_link_poll()` regularly, both with the current time. Requests arrive in
`on_message()`.

### Answering

Answer from `on_message()` or later from the application, with `isotp_link_send()`. The
answer is copied, so a local buffer is fine:

```c
static struct isotp_link link;

static void on_message(const uint8_t* data, uint32_t length, void* context)
{
    (void) context;
    if ((length >= 1U) && (data[0] == 0x3EU))   /* UDS TesterPresent */
    {
        const uint8_t answer[2] = {0x7EU, 0x00U};
        (void) isotp_link_send(&link, answer, sizeof(answer), board_micros());
    }
}
```

`isotp_link_send()` returns `ISOTP_BUSY` while a previous long answer is still being sent.
A request–response protocol such as UDS never has two answers in flight; if your
application can, keep the second answer and send it from `on_tx_done()`.

### Rules a well-behaved ECU follows

- Poll the link at least as often as the shortest separation time a tester may ask for —
  usually every millisecond.
- Answer within the time the application protocol allows (UDS: P2server, 50 ms by
  default); a long answer only has to *start* in time.
- Do not answer functional requests that do not need an answer: this is decided by the
  application layer, not by ISO-TP.
- Keep the receive buffer as large as the longest request you accept; longer requests are
  refused with Flow Control *Overflow*.

## Writing a tester

A tester sends a request and waits for the answer or for an error:

```c
static struct isotp_link tester;
static volatile bool done;

static void on_answer(const uint8_t* data, uint32_t length, void* context)
{
    (void) data;
    (void) length;
    (void) context;
    done = true;
}

static void on_error(int n_result, void* context)
{
    (void) context;
    if (n_result != ISOTP_N_OK)
    {
        done = true;    /* timeout, overflow, wrong SN, ... */
    }
}

int request_vin(void)
{
    static const uint8_t request[3] = {0x22U, 0xF1U, 0x90U};   /* ReadDataByIdentifier */
    uint8_t frame[8];
    uint32_t can_id;
    const uint32_t start = board_micros();
    int result = isotp_link_send(&tester, request, sizeof(request), start);

    done = false;
    while ((result == ISOTP_OK) && !done && ((board_micros() - start) < 2000000U))
    {
        size_t length = board_can_read(&can_id, frame);
        if ((length > 0U) && (can_id == 0x7E8U))
        {
            isotp_link_receive(&tester, frame, length, board_micros());
        }
        isotp_link_poll(&tester, board_micros());
    }
    return result;
}
```

with `on_answer` as `on_message` and `on_error` as both `on_tx_done` and `on_rx_error` in
the tester's configuration (`tx_id` 0x7E0). A functional request goes to all ECUs at once
and must fit a Single Frame:

```c
static struct isotp_link tester;

int tester_present_to_all(void)
{
    static const uint8_t request[2] = {0x3EU, 0x80U};   /* suppress positive response */
    return isotp_link_send_functional(&tester, 0x7DFU, request, sizeof(request));
}
```

## Using the codec without a link

The codec is useful on its own, for example in a bus monitor or a gateway that only looks
at frames:

```c
void show(const uint8_t* data, size_t length)
{
    struct isotp_pdu pdu;

    if (isotp_decode(data, length, &pdu) == ISOTP_OK)
    {
        if (pdu.type == ISOTP_PCI_FIRST_FRAME)
        {
            /* a message of pdu.message_length bytes starts here */
        }
        else if (pdu.type == ISOTP_PCI_FLOW_CONTROL)
        {
            uint32_t separation_us = isotp_st_min_decode(pdu.st_min);
            (void) separation_us;
        }
        else
        {
            /* Single or Consecutive Frame */
        }
    }
}
```

## Return values

| Function | Success | Failure |
| --- | --- | --- |
| `isotp_decode()` | `ISOTP_OK` | `ISOTP_ERROR`: `NULL`, length 0 or > 8, malformed frame |
| `isotp_encode_*()`, `isotp_frame_pad()` | frame length (1..8) | `ISOTP_ERROR`: `NULL`, value out of range, buffer too small |
| `isotp_link_init()` | `ISOTP_OK` | `ISOTP_ERROR`: `NULL`, no writer, value out of range |
| `isotp_link_send()` | `ISOTP_OK` | `ISOTP_BUSY`, `ISOTP_ERROR`, or the writer's error |
| `isotp_link_send_functional()` | `ISOTP_OK` | `ISOTP_ERROR` (longer than 7 bytes), or the writer's error |
| `isotp_link_tx_result()`, `isotp_link_rx_result()` | `ISOTP_N_OK` | an `ISOTP_N_*` code |

## Diagnostics

| Function | Tells you |
| --- | --- |
| `isotp_link_tx_busy()` | A segmented transmission is in progress |
| `isotp_link_tx_result()` | The outcome of the last transmission |
| `isotp_link_rx_result()` | The outcome of the last reception (also `ISOTP_N_UNEXP_PDU` when a new request interrupted one) |
| `isotp_link_invalid_frames()` | How many malformed frames were ignored — a high count points to wrong CAN ID routing or a peer with another addressing mode |

## Common mistakes

| Symptom | Likely cause |
| --- | --- |
| Single Frames work, long messages time out at the peer (N_Bs) | Frames of the peer's ID are not passed to `isotp_link_receive()`, or the Flow Control is sent with the wrong `tx_id` |
| Long messages stall after the First Frame | `isotp_link_poll()` is not called, or called with a time that does not advance |
| Every transmission ends with `ISOTP_N_TIMEOUT_BS` at once | `now_us` is in milliseconds or wraps at less than 2^32 |
| The peer reports wrong sequence numbers | Frames are dropped by a full CAN RX queue; enlarge it or ask for a larger STmin |
| The peer ignores the frames | It expects 8-byte frames: set `padding` |
| `isotp_link_invalid_frames()` grows | Another node's traffic is routed to the link, or the peer uses extended addressing |
| `isotp_link_send()` returns `ISOTP_BUSY` forever | The previous transmission waits for Flow Control that never comes; it ends after N_Bs |
