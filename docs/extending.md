# Extending the Library

Almost everything an application needs is built on top of ISOTP-C: a different CAN
driver, more peers, diagnostic services, gateways. This chapter shows where each kind of
extension belongs, what would need a library change, and how to contribute one.

## Where to extend

| You want to… | Do it in | Library change needed? |
| --- | --- | --- |
| Use another CAN driver or controller | your write function and receive loop | No |
| Talk to several ECUs | one `struct isotp_link` per peer, route frames by CAN ID | No |
| Use 29-bit CAN IDs (for example normal fixed addressing 0x18DAxxyy) | `tx_id` and your routing; the writer sets the IDE flag | No |
| Implement UDS, OBD or a bootloader | `on_message()` and the application | No |
| Route ISO-TP between two buses (gateway) | two links and the application | No |
| Change buffer sizes or defaults | compiler definitions, see [Configuration](configuration.md) | No |
| Extended or mixed addressing (address byte before N_PCI) | the codec and the link | Yes |
| CAN FD (frames of up to 64 bytes) | the codec and the link | Yes |
| Several messages in flight per link | not planned: ISO-TP is one message per direction per link | — |

## Adding a CAN driver

The write function receives the CAN ID and 1 to 8 data bytes and must return:

| Return | Meaning for the link |
| --- | --- |
| `0` | The frame is queued for transmission |
| `ISOTP_BUSY` | No free transmit mailbox: a Consecutive Frame is retried on the next poll; `isotp_link_send()` returns it for a Single or First Frame |
| any other value | Driver error: a transmission ends with `ISOTP_N_ERROR`, a reception with `ISOTP_N_ERROR` when a Flow Control frame cannot be sent |

The data is only valid during the call: copy it into the mailbox or your queue. Do not
block for long — the link calls the writer from `isotp_link_receive()` to send Flow
Control, and a slow writer delays the frames behind it.

## Talking to several ECUs

Declare one link per peer and dispatch on the CAN ID:

```c
static struct isotp_link engine;
static struct isotp_link brake;

void on_can_frame(uint32_t can_id, const uint8_t* data, size_t length, uint32_t now_us)
{
    if (can_id == 0x7E8U)
    {
        isotp_link_receive(&engine, data, length, now_us);
    }
    else if (can_id == 0x7E9U)
    {
        isotp_link_receive(&brake, data, length, now_us);
    }
    else
    {
        /* not ISO-TP traffic for us */
    }
}
```

Poll every link from the same loop.

## Adding an addressing mode or CAN FD

Both change the frame layout: extended and mixed addressing put an address byte in front
of the N_PCI, CAN FD allows frames of up to 64 bytes with an escape sequence for the Single
Frame length. A contribution would add the layout to `struct isotp_link_config` (so one
build serves all modes), extend `isotp_decode()` and the encoders with the offset and the
maximum frame length, extend the Python twin the same way, and add the reference frames
of the standard to the unit and integration tests.

## Contributing to the library

1. **Code.** C11, no new dependencies, no heap, the MISRA rules used in the code (one
   `return` per function, parenthesized expressions, `(void)` casts, no pointer
   arithmetic).
2. **Tests.** A unit test for every behavior; 100 % line and branch coverage; update the
   Python twin and the integration tests when the wire format changes.
3. **Analysis.** `misra-analyze` reports 0 violations, or the deviation is in the
   [register](misra.md).
4. **Format.** `clang-format` reports no changes.
5. **Documentation.** The affected pages, the [stack table](configuration.md#stack-usage),
   the diagrams (`tools/docs/diagrams.py`) and `CHANGELOG.md` `[Unreleased]`.
6. **Compatibility.** The public API is stable within a major version.

The local definition of done before a pull request:

```bash
cmake --preset "ISOTP[Debug]" && cmake --build "build/ISOTP[Debug]"
cmake --preset "ISOTP[UnitTests]" && cmake --build "build/ISOTP[UnitTests]" --target coverage
python -m pytest tests/integration
cmake --build "build/ISOTP[Debug]" --target misra-analyze
cmake --preset "ISOTP[Examples]" && cmake --build "build/ISOTP[Examples]"
ctest --test-dir "build/ISOTP[Examples]" -L example
python tools/docs/check_snippets.py
```
