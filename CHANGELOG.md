# Changelog

All notable changes to this project are documented in this file.
The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and the
project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

Version 2 is a rewrite of the library to the SystemFromScratch® protocol-library
standard. The public API changed; the wire format is ISO 15765-2 as before.

### Added

- Layered library: `isotp_defs.h` (constants, N_Result codes), `isotp_protocol.h`
  (frame codec: `isotp_decode()`, `isotp_encode_*()`, `isotp_frame_pad()`, STmin) and
  `isotp_link.h` (transport layer), each usable on its own.
- Explicit time input: every link call takes `now_us`; the library no longer calls
  `isotp_user_get_us()` or any other platform function.
- `struct isotp_link_config` with run-time block size, STmin, N_Bs, N_Cr, N_WFTmax and
  padding per link; `isotp_link_config_default()`.
- Callbacks with a context: `on_message()`, `on_tx_done()` (every transmission, with its
  N_Result) and `on_rx_error()`.
- `isotp_link_send_functional()` for functional (broadcast) Single Frames.
- Diagnostics: `isotp_link_tx_busy()`, `isotp_link_tx_result()`, `isotp_link_rx_result()`,
  `isotp_link_invalid_frames()`.
- Examples: bare-metal UDS-style ECU with a simulated tester (CTest), Linux SocketCAN
  request tool, Zephyr application.
- Python twin of the codec with a C/Python cross-check and end-to-end tests of the C link
  against the twin.
- Documentation website in the SystemFromScratch® style, MISRA deviation register,
  measured RAM and stack figures, `CITATION.cff`, release workflow.

### Changed

- **Breaking:** `IsoTpLink` is now `struct isotp_link`; `isotp_init_link()`,
  `isotp_on_can_message()`, `isotp_send()`, `isotp_poll()` are replaced by
  `isotp_link_init()`, `isotp_link_receive()`, `isotp_link_send()`, `isotp_link_poll()`.
- **Breaking:** the message buffers are part of `struct isotp_link`, sized by
  `ISOTP_TX_BUFFER_SIZE` and `ISOTP_RX_BUFFER_SIZE`, instead of caller pointers.
- **Breaking:** received messages are delivered to `on_message()`; the polled
  `isotp_receive()` and the weak `isotp_user_*()` functions are removed.
- **Breaking:** configuration macros renamed from `ISO_TP_*` to `ISOTP_*`; the
  compile-time switches for callbacks, padding and the send argument are now run-time
  configuration.
- Return codes reduced to `ISOTP_OK`, `ISOTP_ERROR`, `ISOTP_BUSY`; protocol outcomes use
  the `ISOTP_N_*` N_Result codes.
- One Consecutive Frame per `isotp_link_poll()` call, with wrap-safe timers.

### Fixed

- The 32-bit FF_DL is encoded and decoded big-endian on every host (it was byte-swapped
  only on little-endian hosts).
- Reserved STmin values are treated as 127 ms, as ISO 15765-2 requires (they were 0).
- The requested STmin is rounded up when encoded, never down.
- Flow Control frames are only accepted while the sender waits for one; an escape First
  Frame announcing 4095 bytes or less is ignored.
- Segmented transmissions are no longer allowed to functional addresses.
- The N_Cr timer works when the deadline is 0 after a clock wrap.
- No `<stdio.h>`, `snprintf()` or `assert()` in the library.

### Quality

- 82 unit tests, 100 % line and branch coverage of `src/` (gate in CI).
- 62 integration tests (pytest) against the Python twin.
- Cppcheck MISRA C:2012 addon: 0 violations (gate in CI); 3 documented advisory
  deviations.

## [1.6.0] and earlier

Versions 1.x of the fork kept the API of isotp-c by Li Shen and Simon Cahill. See the
[Git history](https://github.com/AntonVynohradov/isotp-c/commits/main) and the
[releases](https://github.com/AntonVynohradov/isotp-c/releases).

[Unreleased]: https://github.com/AntonVynohradov/isotp-c/compare/v1.6.0...HEAD
[1.6.0]: https://github.com/AntonVynohradov/isotp-c/releases/tag/v1.6.0
