# Integration Guide

The library does not touch hardware and does not read a clock. An integration has three
parts:

1. **Receive:** move every CAN frame of the peer's CAN ID into `isotp_link_receive()`,
   together with the current time, from a task or the main loop — not from the CAN
   interrupt handler.
2. **Transmit:** a write function that queues one CAN frame, and regular calls of
   `isotp_link_poll()` with the current time.
3. **Application:** `on_message()` for complete messages, and optionally `on_tx_done()`
   and `on_rx_error()` for results.

Complete, buildable programs for each pattern are described in [Examples](examples.md).

## The time source

`now_us` is any free-running microsecond counter that wraps at 2^32: a 32-bit timer
running at 1 MHz, `k_cyc_to_us_floor32(k_cycle_get_32())` on Zephyr, the DWT cycle
counter divided by the core clock in MHz, or `clock_gettime(CLOCK_MONOTONIC)` on Linux. A
millisecond tick multiplied by 1000 also works when your STmin and timeouts are whole
milliseconds; the library only needs the value to grow monotonically and wrap at 2^32.

## RTOS: interrupt, queue and CAN task

```text
CAN RX interrupt ──► message queue ──► CAN task ──► isotp_link_receive() ──► on_message()
                                         │
                                         └──► every 1 ms: isotp_link_poll() ──► write()
```

The interrupt copies the frame (ID, length, data) into a queue and returns. The CAN task
waits on the queue with a timeout of one millisecond, passes each frame of the peer's ID
to the link, and polls the link after every wake-up. Only this task touches the link, so
no mutex is needed. When another task must send, either hand the message to the CAN task
through a second queue, or protect `isotp_link_send()`, `isotp_link_receive()` and
`isotp_link_poll()` of that link with one mutex.

## Adding the library to a CMake build

```cmake
include(path/to/isotp-c/isotp.cmake)
target_link_libraries(app PRIVATE isotp)
target_link_libraries(isotp PUBLIC zephyr_interface)   # Zephyr applications only
```

Override the configuration on the target, for example
`target_compile_definitions(isotp PUBLIC ISOTP_RX_BUFFER_SIZE=512U)`; `PUBLIC` makes the
application see the same `struct isotp_link` size as the library.

## Bare metal and host

**Bare metal.** The CAN interrupt fills a small ring buffer of frames; the main loop
drains it, feeds the link and polls it, then sleeps until the next interrupt. A periodic
timer interrupt (for example SysTick at 1 kHz) wakes the loop often enough for STmin and
the timeouts. The [bare-metal example](examples.md#bare-metal) shows the complete
program.

**Linux.** Use SocketCAN: a raw `CAN_RAW` socket with a filter on the peer's ID, `poll()`
with a timeout of one millisecond, and `clock_gettime(CLOCK_MONOTONIC)`. The kernel also
has its own ISO-TP socket (`CAN_ISOTP`); ISOTP-C is useful on Linux when you need the same
code as on the ECU, or full control over timing in a test bench.

## Notes from hardware bring-up

- **Padding.** Many ECUs and testers expect every frame to have 8 data bytes. If the peer
  ignores your frames, set `padding = true`.
- **Acceptance filters.** Configure the CAN controller to accept the peer's ID (and the
  functional ID in an ECU); everything else costs interrupt time.
- **Transmit mailboxes.** With one mailbox and STmin 0, Consecutive Frames leave as fast
  as the bus allows; return `ISOTP_BUSY` when the mailbox is full and the link retries.
- **Frame order.** Use a single transmit FIFO or one mailbox for ISO-TP: priority-based
  mailboxes can send Consecutive Frames out of order, which the receiver reports as a
  wrong sequence number.
- **RX overruns.** A receiver that asks for STmin 0 and BS 0 receives frames back to back
  (about 4 000 frames per second at 500 kbit/s): make the RX queue deep enough or ask for a
  block size.
