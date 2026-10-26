# Introduction

ISOTP-C is a C11 library that implements the transport protocol of
[ISO 15765-2 (ISO-TP)](https://www.iso.org/standard/84211.html) for classical CAN, for
embedded systems that must be predictable and reviewable. It carries messages of up to
4095 bytes — or longer, with the 32-bit length of ISO 15765-2:2016 — over CAN frames of
eight bytes, which is what diagnostics (UDS, ISO 14229), OBD and CAN bootloaders need.

ISOTP-C started as a fork of the MIT-licensed isotp-c by Li Shen and Simon Cahill.
Version 2 is a rewrite with a new API; see [the changelog](changelog.md) for the
differences.

## What the library does

- **Encodes and decodes frames.** Single, First, Consecutive and Flow Control frames,
  including the 32-bit FF_DL escape, optional padding, and the STmin encoding with its
  reserved values.
- **Segments and reassembles messages.** A link sends a long message as a First Frame
  followed by Consecutive Frames, at the block size and separation time the receiver
  asks for, and reassembles incoming messages while checking every sequence number.
- **Detects protocol errors.** Timeouts N_Bs and N_Cr, wrong sequence numbers,
  unexpected frames, too many Wait frames, overflow and unknown flow status are
  reported as the N_Result codes of ISO 15765-2.
- **Connects the protocol to any CAN driver.** One write callback; received frames and
  the current time are function arguments.

## What the library does not do

| Concern | Why it is not in the library |
| --- | --- |
| CAN drivers, acceptance filters | Hardware- and OS-specific; you pass frames in and provide a write function. |
| Routing CAN IDs to links | Depends on your addressing plan; one link serves one pair of CAN IDs. |
| Clocks and timers | The library never reads a clock: every call takes `now_us`. |
| Threads, queues, interrupts, locking | Every system has its own; see the [integration guide](integration.md). |
| Diagnostic services (UDS, OBD) | Application layer; the [examples](examples.md) show a small UDS-style ECU. |
| Extended/mixed addressing, CAN FD | Not implemented yet; see [Extending the Library](extending.md). |

## Who it is for

- **ECU firmware.** A diagnostic server or bootloader on a microcontroller, bare metal
  or under an RTOS.
- **Testers and gateways.** A diagnostic client on a PC (SocketCAN) or a gateway that
  routes ISO-TP between buses.
- **Test benches.** Simulated nodes that need a protocol implementation with
  deterministic timing (the time is an argument, so tests control it).

## Key properties

| Property | Value |
| --- | --- |
| Language | C11, no warnings with `-Wall -Wextra -Wpedantic -Wconversion` |
| Dependencies | `<stddef.h>`, `<stdint.h>`, `<stdbool.h>`, `<string.h>` only |
| Memory | No heap; caller-owned `struct isotp_link`; buffers sized by `ISOTP_TX_BUFFER_SIZE` / `ISOTP_RX_BUFFER_SIZE` |
| RAM per link | 104 bytes + both buffers (8 296 bytes with the default 4095-byte buffers, Cortex-M4) |
| Stack | 120 bytes worst case on Cortex-M4 `-Os` ([details](configuration.md#stack-usage)) |
| Code size | 1 784 bytes of flash on Cortex-M4 `-Os` (codec 516, link 1 268) |
| Coding standard | MISRA C:2012, 0 Cppcheck addon violations, [3 documented deviations](misra.md) |
| Tests | 82 unit tests, 100 % line and branch coverage; 62 integration tests against a Python twin |
| Thread safety | None built in, by design; see [Threading and reentrancy](architecture.md#threading-and-reentrancy) |
| License | [MIT License](LICENSE.md) |

## Terminology

Link
:   One ISO-TP connection to one peer: the CAN ID this node sends with and the frames
    the application routes to it. `struct isotp_link` holds its state.

Message (N_SDU)
:   The payload the application sends or receives, 1 to `ISOTP_TX/RX_BUFFER_SIZE` bytes.

SF, FF, CF, FC
:   Single Frame, First Frame, Consecutive Frame and Flow Control frame — the four
    frame types (N_PDU) of ISO 15765-2.

FF_DL, SF_DL
:   The message length announced in a First Frame or Single Frame.

SN
:   Sequence number of a Consecutive Frame, 1, 2, … 15, 0, 1, …

BS, STmin
:   Block size and minimum separation time: how many Consecutive Frames the sender may
    send before the next Flow Control frame, and how long it waits between them.

N_Bs, N_Cr
:   Sender timeout waiting for Flow Control; receiver timeout waiting for the next
    Consecutive Frame.

N_WFTmax
:   How many Flow Control frames with the status Wait in a row the sender accepts.

Physical and functional addressing
:   A request to one node, or a broadcast request (only as a Single Frame).

## Conventions in this documentation

- Hexadecimal bytes without prefix inside frames (`10 14 00 01`), with `0x` in text and code.
- The 32-bit FF_DL is big-endian, as on the wire.
- Times are in microseconds (`_us`) unless a unit is given.
- `ISOTP_TX_BUFFER_SIZE` and `ISOTP_RX_BUFFER_SIZE` are shown with their default, 4095.
