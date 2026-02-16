# API Reference

This API reference is generated from the source code with Doxygen and mirrors the
public headers and their supporting modules.

## Main Components

- **[isotp.h](docs/isotp_8h.md)** - Public ISO-TP API: link lifecycle, send/receive, and polling.
- **[isotp_config.h](docs/isotp__config_8h.md)** - Build-time configuration parameters and feature toggles.
- **[isotp_user.h](docs/isotp__user_8h.md)** - User hooks for CAN transmit, timing, and debug output.
- **[isotp_defines.h](docs/isotp__defines_8h.md)** - Internal protocol definitions, frame layouts, and enums.

## Tests

- **[isotp_test_support.h](docs/isotp__test__support_8h.md)** - Test utilities used by unit tests.

## Examples

- **[linux_socket.c](docs/linux__socket_8c.md)** - Linux SocketCAN example showing end-to-end usage.

## Modules and Functions

Browse the [modules overview](docs/modules.md) for a grouped view of public interfaces,
configuration, and user hooks. Each module page links to the relevant functions and
data structures.

## Data Structures

Core ISO-TP structures, such as the link state and frame types, are documented in
the generated API pages referenced above.

For more details on protocol behavior, see ISO 15765-2.
