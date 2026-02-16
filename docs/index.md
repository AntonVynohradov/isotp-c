# ISO-TP C Implementation

ISO-TP (ISO 15765-2) protocol implementation in C for CAN communication.
This library implements ISO-TP frame fragmentation and reassembly and provides
a simple interface for sending and receiving messages longer than 8 bytes.

## Key features

- Full ISO-TP support (Single, First, Consecutive, Flow Control)
- Configurable timing parameters and buffer sizes
- Minimal dependencies and easy integration in embedded systems
- Main-loop (polling) operation, no mandatory OS required
- Optional callbacks for TX and RX completion

## Intended use

The library is intended for embedded systems and CAN applications where
deterministic behavior, small memory footprint, and simple integration are required.

## Quick start

### Build (CMake)

This project uses CMake presets. You can build with the default generator or
use the provided presets for Debug, Release, and Unit Tests.

Quick local build:

```bash
cmake -B build
cmake --build build
```

Build with presets:

```bash
cmake --preset "ISO-TP[Debug]" -B "build/ISO-TP[Debug]"
cmake --build "build/ISO-TP[Debug]"

cmake --preset "ISO-TP[Release]" -B "build/ISO-TP[Release]"
cmake --build "build/ISO-TP[Release]"
```

Unit tests (requires Google Test):

```bash
cmake --preset "ISO-TP[UnitTests]" -B "build/ISO-TP[UnitTests]"
cmake --build "build/ISO-TP[UnitTests]"
ctest --test-dir "build/ISO-TP[UnitTests]" --output-on-failure
```

### Integration

Add the header and configure user functions in `isotp_user.h`:

```c
#include "isotp.h"

static uint8_t tx_buffer[4095];
static uint8_t rx_buffer[4095];
static IsoTpLink link;

isotp_init_link(&link, 0x7E0, tx_buffer, sizeof(tx_buffer),
								rx_buffer, sizeof(rx_buffer));
```

In the application loop:

```c
isotp_on_can_message(&link, can_data, can_len);
isotp_poll(&link);

uint8_t payload[4095];
uint32_t payload_len = 0;
if (isotp_receive(&link, payload, sizeof(payload), &payload_len) == ISOTP_RET_OK) {
		/* process payload */
}
```

## Core API

Key public functions (see [API Reference](api.md)):

- `isotp_init_link()` - link initialization
- `isotp_destroy_link()` - release link resources
- `isotp_send()` / `isotp_send_with_id()` - send ISO-TP messages
- `isotp_on_can_message()` - handle incoming CAN frames
- `isotp_poll()` - handle timeouts and send consecutive frames
- `isotp_receive()` - fetch reassembled payload

## Configuration

Configuration is done statically via files in `inc/`:

- `inc/isotp_config.h` - timing parameters, sizes, enable/disable options
- `inc/isotp_user.h` - user functions (CAN send, time, debug)
- `inc/isotp_defines.h` - protocol definitions and error codes

More details in [Configuration Guide](configuration.md).

## Usage model

- The library does not manage the CAN bus; it expects a CAN send function
	and delivery of received frames via `isotp_on_can_message()`.
- The `isotp_poll()` function must be called periodically in the main loop
	(or in a thread/RTOS) at an appropriate frequency.

## Examples

The Linux SocketCAN example is in
[`example/linux_socket/linux_socket.c`](examples/linux_socket.md).

## Tests

Unit tests are located in the `tests/` directory. Details are in the project
documentation and the test files.

To run unit tests locally (requires Google Test):

```bash
cmake --preset "ISO-TP[UnitTests]" -B "build/ISO-TP[UnitTests]"
cmake --build "build/ISO-TP[UnitTests]"
ctest --test-dir "build/ISO-TP[UnitTests]" --output-on-failure
```

Install Google Test on Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install -y libgtest-dev
```

## Limitations and notes

- The maximum payload length depends on configuration and buffers.
- Provide sufficiently large TX/RX buffers (typically 4095 bytes for full ISO-TP).
- For realtime systems, adjust timeouts and polling frequency.

## Documentation

- [API Reference](api.md)
- [Configuration Guide](configuration.md)
- [Getting Started](guide.md)

## License

Details in [LICENSE](LICENSE.md).
