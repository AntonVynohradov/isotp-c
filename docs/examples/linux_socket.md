# Linux socket

## How to use

This example shows how to run ISO-TP-C on Linux SocketCAN. Below are the build
steps and a short explanation of what the program does.

## Requirements

- Linux with SocketCAN enabled.
- A configured CAN interface (default: `slcan0`).
- C99 compiler and CMake >= 3.20.

## Example configuration

In example/linux_socket/linux_socket.c the key parameters are defined:

- `_CAN_INTERFACE` - CAN interface name (e.g., `can0`, `slcan0`).
- `_ISOTP_CAN_ID` - CAN ID used for ISO-TP transmission.
- `_ISOTP_BUFSIZE` - TX/RX buffer size (>= 4095 for full ISO-TP support).

## Build

From the example directory:

```bash
cd example/linux_socket
cmake -G "Unix Makefiles" -B build
cmake --build build
```

## Run

After building, run the application:

```bash
./build/LinuxSocketExample
```

The program listens on CAN, assembles ISO-TP frames, and prints the received payload.

## What the program does

- Creates a `PF_CAN`/`SOCK_RAW` socket and binds it to the CAN interface.
- Initializes the ISO-TP link via `isotp_init_link()`.
- Receives CAN frames in a loop, passes them to `isotp_on_can_message()`, and
  calls `isotp_poll()`.
- When `receive_status` is `ISOTP_RECEIVE_STATUS_FULL`, reads data with
  `isotp_receive()` and prints it as hex.

## Notes

- The CAN interface and CAN ID must match the configuration on the other side.
- The example uses extended identifiers (`CAN_EFF_FLAG`).