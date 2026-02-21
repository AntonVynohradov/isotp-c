# ISO-15765: Project Structure and Integration Test Coverage

This document describes the project structure for ISO-15765 (ISO-TP) tests and
the current integration test coverage. It highlights what is implemented and
verified, and where there are gaps in implementation or tests.

## Project Structure (Relevant to ISO-TP and Tests)

- ISO-TP core (protocol implementation): [src/isotp.c](docs/isotp_8c.md)
- Public API and link model: [isotp.h](docs/isotp_8h.md)
- Protocol definitions and result codes: [inc/isotp_defines.h](docs/isotp__defines_8h.md)
- Build-time configuration (BS, STmin, timeouts, options): [inc/isotp_config.h](docs/isotp__config_8h.md)
- Platform hooks (CAN, time, debug): [inc/isotp_user.h](docs/isotp__user_8h.md)
- Python bindings and CAN/time mocks (test layer): [examples/python_api/src](docs/pyisotp_8c.md)
- Integration tests (Python): tests/integration
- Integration test documentation: docs/integration_tests

## Implemented and Verified by Integration Tests

The following areas are implemented in the core and covered by integration tests:

- API smoke + input errors + return codes
    - Tests: [tests/integration/test_api_endpoints.py](docs/namespacetest__api__endpoints.md)
    - Docs: [integration_tests/api_endpoints.md](integration_tests/api_endpoints.md)
    - Scope: SF/MF smoke, sender N_Bs timeout, invalid args, overflow, in-progress.

- API validation (arguments)
    - Tests: [tests/integration/test_api_validation.py](docs/namespacetest__api__validation.md)
    - Docs: [integration_tests/api_validation.md](integration_tests/api_validation.md)
    - Scope: zero buffers, bufsize=0, block_size > 255.

- Buffer limits and overflow handling
    - Tests: [tests/integration/test_buffer_limits.py](docs/namespacetest__buffer__limits.md)
    - Docs: [integration_tests/buffer_limits.md](integration_tests/buffer_limits.md)
    - Scope: ISOTP_RET_OVERFLOW, truncation on receive, ISOTP_RET_INPROGRESS.

- Multi-frame behavior (FF/CF, SN, BS, STmin, timeouts)
    - Tests: [tests/integration/test_multi_frame.py](docs/namespacetest__multi__frame.md)
    - Docs: [integration_tests/multi_frame.md](integration_tests/multi_frame.md)
    - Scope: MF roundtrip, 8/9/14/15 boundaries, max payload 4095B,
      BS=1 and BS=0, STmin, N_Bs/N_Cr timeouts, UNEXP_PDU and WRONG_SN errors.

- FlowControl (FS=WAIT/CONTINUE/OVERFLOW)
    - AUTOSAR CanTp: [tests/integration/test_cantp.py](docs/namespacetest__cantp.md),
    - Docs: [integration_tests/cantp.md](integration_tests/cantp.md),
    - Tests: [tests/integration/test_flow_control.py](docs/namespacetest__flow__control.md)
    - Docs: [integration_tests/flow_control.md](integration_tests/flow_control.md)
    - Scope: WAIT -> CONTINUE, WFT overrun, FS=Overflow, BS and STmin parameters.

- Timing (N_Bs, N_Cr, STmin, BS)
    - Tests: [tests/integration/test_timing.py](docs/namespacetest__timing.md)
    - Docs: [integration_tests/timing.md](integration_tests/timing.md)
    - Scope: N_Bs and N_Cr timeouts, STmin enforcement, BS=1 roundtrip.

## Implementation Gaps (Explicitly Identified)

- N_As, N_Ar, N_Br, N_Cs are not enforced in the core yet (marked as xfail
  in timing tests). See: [tests/integration/test_timing.py](docs/namespacetest__timing.md)
  and [integration_tests/timing.md](integration_tests/timing.md).

## Integration Test Gaps (Areas to Expand)

Based on current integration tests, there is no visible coverage for:

- `isotp_send_with_id` (functional addressing) in [isotp.h](docs/isotp_8h.md).
- `tx_done_cb` and `rx_done_cb` callbacks (when enabled in config)
  in [isotp.h](docs/isotp_8h.md) and [inc/isotp_config.h](docs/isotp__config_8h.md).
- ISO_TP_FRAME_PADDING (padded frames) in [inc/isotp_config.h](docs/isotp__config_8h.md).
- `ISOTP_RET_NOSPACE` scenarios (retry send) in [inc/isotp_defines.h](docs/isotp__defines_8h.md).
- Invalid FlowControl (e.g., invalid FS) and other protocol errors
  beyond UNEXP_PDU and WRONG_SN in [inc/isotp_defines.h](docs/isotp__defines_8h.md).
- Transfers larger than 4095B (long FF) - no integration tests yet.

## Notes

- Integration tests are executed via Python bindings and CAN/time mocks
  (see [examples/python_api/src](docs/pyisotp_8c.md)).
- This document only covers integration test coverage. Unit tests may cover
  additional areas.
