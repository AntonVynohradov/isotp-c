# Examples

Three examples show the patterns of the [Integration Guide](integration.md). The bare-metal
example is also a test: CI runs it on a simulated board.

| Example | Role | Platform | Built and run in CI |
| --- | --- | --- | --- |
| [Bare metal](#bare-metal) | ECU | Any MCU; runs on a PC against a simulated tester | Yes, as a self-checking test |
| [Linux SocketCAN](#linux-socketcan) | Tester | Linux | Built |
| [Zephyr](#zephyr) | ECU | Any Zephyr board with a CAN controller | No, needs a Zephyr workspace |

```bash
cmake --preset "ISOTP[Examples]"
cmake --build "build/ISOTP[Examples]"
ctest --test-dir "build/ISOTP[Examples]" -L example --output-on-failure
```

## Bare metal

A UDS-style ECU on request ID 0x7E0 (and functional ID 0x7DF) that answers on 0x7E8. It
supports TesterPresent, and reading and writing a 17-byte VIN with Read/WriteDataByIdentifier
0xF190 — so the response to a read and the write request are both segmented — and answers
other services with a negative response.

| File | Contents |
| --- | --- |
| `examples/bare_metal/main.c` | The ECU: CAN RX interrupt → ring buffer → main loop → link → services |
| `examples/bare_metal/board.h` | The four functions to port: `board_init()`, `board_can_write()`, `board_micros()`, `board_idle()` |
| `examples/bare_metal/board_host.c` | Simulated board for the PC: a tester with its own link on a virtual bus |

The ECU asks for a block size of 2 and an STmin of 1 ms; the simulated tester asks for a
block size of 4 and an STmin of 2 ms, so both directions use several Flow Control frames.
Before every request the tester injects a malformed frame on the request ID and a frame
of another node. The program exits with 0 when every response matches. Real output:

```text
Simulated CAN bus at 500000 bit/s: ECU 0x7E0/0x7E8, tester with BS 4, STmin 2 ms
  request:  3E 00
  response: 7E 00
TesterPresent (functional)           OK
  request:  22 F1 90
  response: 62 F1 90 53 46 53 49 53 4F 54 50 43 32 30 30 30 30 30 30 31
ReadDataByIdentifier VIN             OK
  request:  2E F1 90 53 46 53 49 53 4F 54 50 43 32 30 30 30 30 30 30 32
  response: 6E F1 90
WriteDataByIdentifier VIN            OK
  request:  22 F1 90
  response: 62 F1 90 53 46 53 49 53 4F 54 50 43 32 30 30 30 30 30 30 32
ReadDataByIdentifier VIN again       OK
  request:  10 01
  response: 7F 10 11
Unsupported service                  OK
  request:  3E 80
TesterPresent, response suppressed   OK
0 of 6 exchanges failed
```

**Porting.** Implement `board.h` for your MCU:

| Function | On an STM32 with bxCAN, for example |
| --- | --- |
| `board_init()` | Clock and pins, bit timing, filters for 0x7E0 and 0x7DF, enable the FIFO 0 interrupt, SysTick at 1 kHz |
| `board_can_write()` | Put the frame into a free TX mailbox; `ISOTP_BUSY` when none is free |
| `board_micros()` | A 32-bit timer at 1 MHz, or the DWT cycle counter / (core clock in MHz) |
| `board_idle()` | `__WFI()`; the SysTick interrupt wakes it every millisecond |
| FIFO 0 interrupt | Read the frame and call `app_can_rx_isr()` |

## Linux SocketCAN

`isotp_request` sends one request from a PC and prints the response:

```bash
isotp_request can0 7E0 7E8 22 F1 90
```

| File | Contents |
| --- | --- |
| `examples/linux_socketcan/main.c` | Raw CAN socket with a filter on the response ID, `poll()` every millisecond, `CLOCK_MONOTONIC` |

It pads frames to 8 bytes, waits up to 2 s for the response and prints the N_Result on
an error. Without hardware, create a virtual bus with
`sudo ip link add dev vcan0 type vcan && sudo ip link set up vcan0` and run an ECU on it.

## Zephyr

The ECU on the Zephyr CAN API, for the CAN controller chosen as `zephyr,canbus`:

```bash
west build -b <board> examples/zephyr
```

| File | Contents |
| --- | --- |
| `examples/zephyr/src/main.c` | Filters for 0x7E0/0x7DF into a `k_msgq`, one thread that feeds and polls the link every millisecond, `can_send()` with `-EAGAIN` mapped to `ISOTP_BUSY` |
| `examples/zephyr/CMakeLists.txt` | Includes `isotp.cmake` and links the library to `zephyr_interface` |
| `examples/zephyr/prj.conf` | `CONFIG_CAN=y`, no heap |

It answers TesterPresent and replies *serviceNotSupported* to everything else; add your
services in `on_request()`.
