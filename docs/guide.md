# Getting Started

This guide shows how to initialize an ISO-TP link, send and receive payloads,
and integrate the polling model into your main loop. For full definitions and
return codes, see the [API Reference](api.md).

## Initialization

Create a link and provide dedicated TX/RX buffers. The link tracks protocol
state and owns the buffers you pass in.

```c
#include "isotp.h"

static uint8_t tx_buffer[4095];
static uint8_t rx_buffer[4095];
static IsoTpLink link;

isotp_init_link(&link, 0x7E0, tx_buffer, sizeof(tx_buffer),
				rx_buffer, sizeof(rx_buffer));
```

The `sendid` you pass to `isotp_init_link()` is used for transmissions. Make
sure the receiver expects this ID.

## Sending Data

Single-frame payloads are sent immediately. Multi-frame payloads are queued and
sent by `isotp_poll()`.

```c
uint8_t data[] = {0x01, 0x02, 0x03};
int ret = isotp_send(&link, data, sizeof(data));
if (ret == ISOTP_RET_OK || ret == ISOTP_RET_INPROGRESS)
{
	/* OK: in progress or already sent */
}
```

Use `isotp_send_with_id()` for functional addressing or when you need to send
with a different arbitration ID.

## Receiving Data

Deliver each incoming CAN frame to the ISO-TP handler and poll periodically.
When a complete payload is available, call `isotp_receive()` to copy it out.

```c
isotp_on_can_message(&link, can_data, can_len);
isotp_poll(&link);

uint8_t payload[4095];
uint32_t payload_len = 0;
if (isotp_receive(&link, payload, sizeof(payload), &payload_len) == ISOTP_RET_OK)
{
	/* process payload */
}
```

## Polling Model

ISO-TP timing is managed by `isotp_poll()`. Call it from the main loop or a
dedicated thread with a consistent period that matches your timing needs. The
poll function drives consecutive frame sending and timeout handling.

## User Hooks

Implement user hooks in `isotp_user.h` for:

- CAN frame transmission (required)
- Microsecond time source (required)
- Optional debug output
- Optional TX/RX completion callbacks (when enabled in configuration)

## Example Flow (Linux SocketCAN)

The Linux example creates a `PF_CAN`/`SOCK_RAW` socket, binds it to a CAN
interface, and then runs the ISO-TP loop:

1. Initialize the SocketCAN interface and bind the socket.
2. Call `isotp_init_link()` with buffers and the transmit ID.
3. In a loop, read CAN frames, pass them to `isotp_on_can_message()`, and call
   `isotp_poll()`.
4. When `isotp_receive()` returns `ISOTP_RET_OK`, the payload is complete.

See the Linux SocketCAN example for a full working reference:
[example/linux_socket/linux_socket.c](examples/linux_socket.md).

## Configuration

See [Configuration Guide](configuration.md) for detailed configuration options.

## Static Analysis (Cppcheck)

The project provides a CMake target that runs Cppcheck over the source tree.
Build the `cppcheck` target from your build directory:

```bash
cmake --build <build-dir> --target cppcheck
```

The output is written to `cppcheck.txt` in the build directory and is also
printed to the console after the run completes.

To exclude paths from analysis, edit `cppcheck-excludes.txt` at the project
root. List one path per line. Empty lines are ignored.

## Static Analysis (CodeQL)

The project provides a `codeql-analyze` CMake target that creates a CodeQL
database and runs the standard C/C++ query pack.

Prerequisites:

- Install the CodeQL CLI and ensure `codeql` is available on your PATH.

Run the analysis from your build directory:

```bash
cmake --build <build-dir> --target codeql-analyze
```

What it does:

- Creates a database at `<build-dir>/codeql-db`.
- Builds the library using the `codeql-build` target to avoid recursive runs.
- Runs the `codeql/cpp-queries` pack and writes SARIF to
	`<build-dir>/codeql-report.sarif`.

## Error Handling

The library returns status codes for common conditions:

- `ISOTP_RET_OK` - Success
- `ISOTP_RET_INPROGRESS` - Multi-frame transfer in progress
- `ISOTP_RET_OVERFLOW` - Payload larger than available buffer
- `ISOTP_RET_NO_DATA` - No complete payload available yet

For the full list and detailed meanings, refer to the [API Reference](api.md).
