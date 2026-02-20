# FlowControl Integration Tests

This document describes FlowControl-specific integration tests.
The suite verifies protocol result handling for FlowControl overflow.

## Scope

The tests in tests/integration/test_flow_control.py cover:

- FlowControl Overflow (FS=Overflow) behavior on the sender.

## Test Cases

### test_flow_control_overflow_sets_protocol_result

- Setup: multi-frame payload and injected FlowControl Overflow frame.
- Action: inject FC frame with FS=Overflow.
- Expectation: sender protocol result is ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW.

![FlowControl overflow result](../assets/integration_tests/flow_control/overflow_sets_protocol_result.svg)

## Notes

- Raw CAN injection uses pyisotp.inject_can.
- Update this document if FlowControl result handling changes.
