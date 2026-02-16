# Configuration Guide

## isotp_config.h

The `isotp_config.h` file defines compile-time configuration options that control
ISO-TP timing, limits, and optional features. These settings are applied at build
time and affect both send and receive behavior for single- and multi-frame traffic.

### Key Configuration Options

- `ISO_TP_DEFAULT_BLOCK_SIZE` - Default receiver block size (BS). This limits how many
	consecutive frames can be sent before the receiver responds with Flow Control.
- `ISO_TP_DEFAULT_ST_MIN_US` - Default minimum separation time (ST_min) in microseconds
	between consecutive frames.
- `ISO_TP_MAX_WFT_NUMBER` - Maximum number of consecutive Flow Control Wait frames the
	receiver may send.
- `ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US` - Default response timeout (microseconds) while
	waiting for Flow Control during a multi-frame transfer.
- `ISO_TP_FRAME_PADDING` - Enable ISO-TP frame padding by default.
- `ISO_TP_FRAME_PADDING_VALUE` - Padding byte value used when padding is enabled.
- `ISO_TP_USER_SEND_CAN_ARG` - Adds an extra user argument to `isotp_user_send_can()`.
- `ISO_TP_TRANSMIT_COMPLETE_CALLBACK` - Enable transmission complete callback support.
- `ISO_TP_RECEIVE_COMPLETE_CALLBACK` - Enable receive complete callback support.

## User Callbacks

Define user callbacks in `isotp_user.h` for:
- CAN frame transmission
- Timing source access (microsecond timebase)
- Optional debug output
- Optional transmit/receive completion hooks (when enabled)

## More Information

See `inc/isotp_config.h` for detailed configuration parameters and inline
documentation for each option.
