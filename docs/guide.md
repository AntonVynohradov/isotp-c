# Usage Guide

## Initialization

Initialize the ISO-TP protocol handler:

```c
isotp_t handler;
// Initialize with appropriate parameters
```

## Sending Data

Transmit ISO-TP messages:

```c
uint8_t data[] = {/* your data */};
// Use isotp send functions
```

## Receiving Data

Handle incoming ISO-TP frames:

```c
// Use isotp receive functions
```

## Configuration

See [Configuration Guide](configuration.md) for detailed configuration options.

## Error Handling

The library provides error codes for various conditions:
- Timeout errors
- Protocol errors
- Frame validation errors

For details, refer to the [API Reference](api.md).
