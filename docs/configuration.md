# Configuration

Build-time parameters live in `inc/isotp_config.h`. Each one is guarded by `#ifndef`
and range-checked at compile time; override it with a compiler definition, for example
`-DISOTP_RX_BUFFER_SIZE=256U`. The two buffer sizes are compiled into
`struct isotp_link`; the other values are only the defaults that
`isotp_link_config_default()` writes into a `struct isotp_link_config`, so every link
can use its own values at run time.

| Parameter | Default | Range | Meaning |
| --- | --- | --- | --- |
| `ISOTP_TX_BUFFER_SIZE` | `4095U` | 8 … 2^31−1 | Largest message a link sends, in bytes |
| `ISOTP_RX_BUFFER_SIZE` | `4095U` | 8 … 2^31−1 | Largest message a link receives; longer ones are refused with FC OVFLW |
| `ISOTP_DEFAULT_BLOCK_SIZE` | `8U` | 0 … 255 | BS this receiver requests; 0 = no further Flow Control |
| `ISOTP_DEFAULT_ST_MIN_US` | `0U` | 0 … 127000 | STmin this receiver requests, µs |
| `ISOTP_DEFAULT_N_BS_TIMEOUT_US` | `1000000U` | 1 … 2^31−1 | N_Bs, µs (ISO 15765-2: 1000 ms) |
| `ISOTP_DEFAULT_N_CR_TIMEOUT_US` | `1000000U` | 1 … 2^31−1 | N_Cr, µs (ISO 15765-2: 1000 ms) |
| `ISOTP_DEFAULT_WFT_MAX` | `10U` | 0 … 255 | N_WFTmax: Wait frames accepted in a row |

Messages longer than 4095 bytes use the 32-bit FF_DL of ISO 15765-2:2016; they need a
buffer size above 4095. Your peer must support the escape sequence too.

## Run-time configuration

`struct isotp_link_config` also holds what has no build-time default:

| Member | Default | Meaning |
| --- | --- | --- |
| `tx_id` | 0 | CAN ID of every frame this link sends (11- or 29-bit, passed to the writer) |
| `write`, `write_context` | none | Frame writer (required) and its context |
| `on_message`, `on_tx_done`, `on_rx_error`, `app_context` | none | Callbacks and their context |
| `padding`, `padding_value` | false, `0xCC` | Pad every sent frame to 8 bytes |

`isotp_link_init()` rejects a configuration without a writer, an STmin above 127 000 µs
and timeouts of 0 or above 2^31−1 µs (deadlines are compared modulo 2^32).

## Memory footprint

Measured with `sizeof` and `arm-none-eabi-size` (Cortex-M4, `-Os`):

| Object | Size |
| --- | --- |
| `struct isotp_link`, default 4095-byte buffers | 8 296 bytes |
| `struct isotp_link`, 256-byte buffers | 616 bytes |
| `struct isotp_link`, 64-byte buffers | 232 bytes |
| Per link without the buffers | 104 bytes |
| `struct isotp_link_config` | 48 bytes |
| Code: `isotp_protocol.o` + `isotp_link.o` | 516 + 1 268 = 1 784 bytes of flash, no data, no bss |

The library has no global variables: the RAM it uses is the `struct isotp_link`
instances you declare (one per peer) and the stack.

## Stack usage

Worst-case stack of each public function, including the library functions it calls,
measured with `-fstack-usage -fcallgraph-info=su` and `tools/docs/stack_usage.py`,
default configuration, in bytes:

| Function | Cortex-M4 `-Os` | Cortex-M4 `-O0` | x86-64 `-O2` |
| --- | ---: | ---: | ---: |
| `isotp_decode()` | 12 | 80 | 8 |
| `isotp_encode_consecutive()` | 16 | 32 | 8 |
| `isotp_encode_first()` | 12 | 48 | 8 |
| `isotp_encode_flow_control()` | 0 | 32 | 8 |
| `isotp_encode_single()` | 16 | 32 | 8 |
| `isotp_frame_pad()` | 16 | 32 | 8 |
| `isotp_link_config_default()` | 0 | 16 | 8 |
| `isotp_link_init()` | 16 | 32 | 16 |
| `isotp_link_invalid_frames()` | 0 | 16 | 8 |
| `isotp_link_poll()` | 80 | 136 | 72 |
| `isotp_link_receive()` | 120 | 200 | 120 |
| `isotp_link_rx_result()` | 0 | 16 | 8 |
| `isotp_link_send()` | 72 | 120 | 88 |
| `isotp_link_send_functional()` | 64 | 112 | 56 |
| `isotp_link_tx_busy()` | 0 | 16 | 8 |
| `isotp_link_tx_result()` | 0 | 16 | 8 |
| `isotp_st_min_decode()` | 0 | 24 | 8 |
| `isotp_st_min_encode()` | 0 | 24 | 8 |

Toolchains: xPack GNU Arm Embedded GCC 14.2.1 (`-mcpu=cortex-m4 -mthumb`) and GCC 13.3.0
(x86-64 Linux).

The figures do not include what runs behind a function pointer: your write function and
your callbacks run on top of the library frame that calls them.

| Path | Worst chain (Cortex-M4 `-Os`) | Stack |
| --- | --- | --- |
| Receive | `isotp_link_receive()` → `isotp_link_send_fc()` → `isotp_link_write()` → `isotp_frame_pad()` | 120 B + write function or callback |
| Poll | `isotp_link_poll()` → `isotp_link_write()` → `isotp_frame_pad()` | 80 B + write function |
| Send | `isotp_link_send()` → `isotp_link_write()` → `isotp_frame_pad()` | 72 B + write function |

`on_message()` runs inside `isotp_link_receive()`; when it answers with
`isotp_link_send()`, add that chain too (Cortex-M4 `-Os`: up to 120 + 72 bytes plus your
own frames).

Re-measure after a change:

```bash
mkdir -p build/su && for f in src/*.c; do
  arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Os -std=c11 -I. -Iinc \
      -fstack-usage -fcallgraph-info=su -c "$f" -o "build/su/$(basename "$f" .c).o"
done
python tools/docs/stack_usage.py build/su --markdown --prefix isotp_
```

## CMake options

| Option | Default | Meaning |
| --- | --- | --- |
| `ISOTP_UNIT_TESTS` | `OFF` | GoogleTest suite and `coverage` target (built with 5000-byte buffers) |
| `ISOTP_INTEGRATION_TESTS` | `OFF` | pytest suite |
| `ISOTP_EXAMPLES` | `OFF` | Host examples and the bare-metal simulation test |
| `ISOTP_COVERAGE_MIN` | `100` | Coverage gate |
| `ISOTP_MISRA_FAIL_ON_VIOLATION` | `ON` | MISRA gate |
| `ISOTP_MISRA_RULE_TEXTS` | empty | Licensed MISRA rule texts |
