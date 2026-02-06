# Configuration Guide

## isotp_config.h

The `isotp_config.h` file contains compile-time configuration options.

### Key Configuration Options

- `ISOTP_MAX_MESSAGE_LENGTH` - Maximum message length
- `ISOTP_TIMEOUT` - Communication timeout
- `ISOTP_PADDING_ENABLED` - Enable/disable frame padding

## User Callbacks

Configure user-defined callbacks in `isotp_user.h` for:
- Frame transmission
- Frame reception
- Timing management

## More Information

See `Inc/isotp_config.h` for detailed configuration parameters.
