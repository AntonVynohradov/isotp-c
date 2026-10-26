# MISRA C:2012 compliance

This page records how the library is checked against MISRA C:2012 and every deviation
from it. It covers the library code only: `src/`, `inc/` and `isotp.h`. Tests, examples
and tools are not in scope.

## Checking

| Check | Where | Gate |
| --- | --- | --- |
| Cppcheck MISRA C:2012 addon | `misra-analyze` target, CI job *MISRA C:2012 Analysis* | Fails on any violation |
| Cppcheck, all checks | `cppcheck-analyze` target, CI job *Cppcheck Analysis* | Report |
| CodeQL `cpp-queries` | `codeql-analyze` target, CI job *CodeQL Analysis* | Report |
| Compiler | `-std=c11 -Wall -Wextra -Wpedantic -Wconversion` | No warnings |
| Manual review | Pull request review, see `.github/.copilot-instructions.md` | Reviewer |

`misra-analyze` fails when the report contains a violation. Set
`-DISOTP_MISRA_FAIL_ON_VIOLATION=OFF` to get the report without failing the build. A
violation that cannot be fixed must be added to the deviation register below before the
pull request is merged. The gate was verified by adding an early `return` (Rule 15.5):
the target reported the violation and failed.

The Cppcheck addon checks most decidable rules, but not the directives or rules that need
knowledge of the whole program or the intent of the code. Those are covered by the manual
review notes below.

## Current status

| Item | Result |
| --- | --- |
| Cppcheck MISRA addon (2.13, the version in CI) | 0 violations |
| Cppcheck `--enable=all` | 0 findings |
| Unit test coverage of `src/` | 100 % lines (335), 100 % branches (213) |
| Deviations | 3, all advisory, listed below |

## Manual review notes

| Guideline | How the library complies |
| --- | --- |
| Dir 4.1 (run-time failures) | Every copy into a message buffer is bounded by an invariant written next to it: a First Frame is only accepted when its FF_DL fits `ISOTP_RX_BUFFER_SIZE`, and every Consecutive Frame copies at most the bytes still missing. `isotp_link_send()` rejects messages longer than `ISOTP_TX_BUFFER_SIZE`. `inc/isotp_config.h` rejects sizes and defaults out of range at compile time. Time arithmetic is unsigned and wraps by design; deadlines are compared modulo 2^32. |
| Dir 4.11 (validity of arguments) | Every public function checks its pointer arguments, lengths and ranges and returns `ISOTP_ERROR` (or ignores the call, for the `void` functions; the accessors return a neutral value for `NULL`). The inline big-endian helpers in `isotp_defs.h` take a pointer to four readable bytes, like `memcpy()`. |
| Dir 4.12, Rule 21.3 (dynamic memory) | No heap. All state is in the caller-owned `struct isotp_link`. |
| Rule 15.5 (single point of exit) | Every function has one `return` at the end. |
| Rule 17.2 (recursion) | No recursion; the stack depth is fixed, see [Configuration](configuration.md#stack-usage). A callback that calls back into the same link (for example `isotp_link_send()` from `on_message()`) is a bounded, one-level re-entry. |
| Rule 21.6 (standard I/O) | No `<stdio.h>`. The library includes only `<stddef.h>`, `<stdint.h>`, `<stdbool.h>` and `<string.h>`. |

## Deviation register

### DEV-01: `int` as the status type (Dir 4.6, advisory)

- **Location:** return type of `isotp_decode()`, the `isotp_encode_*()` functions,
  `isotp_frame_pad()`, `isotp_link_init()`, `isotp_link_send()`,
  `isotp_link_send_functional()`, `isotp_link_tx_result()`, `isotp_link_rx_result()`, of
  the `isotp_can_write_fn` callback and the N_Result argument of `isotp_result_fn`.
- **Rationale:** the value is a status code (`ISOTP_OK`, `ISOTP_ERROR`, `ISOTP_BUSY`, an
  N_Result code, a frame length of at most 8, or the code the write callback returned),
  not a quantity whose size matters. `int` lets an integrator return the error code of an
  existing CAN driver from the write callback without a conversion. `int` is at least 16
  bits on every conforming compiler, which holds every value the library returns.
- **Risk control:** the library never does arithmetic on these values; it compares them
  with the named codes or uses a frame length (1..8) that the encoders produced.

### DEV-02: macros not used inside the library (Rule 2.5, advisory)

- **Location:** `ISOTP_N_TIMEOUT_A` in `inc/isotp_defs.h`, `ISOTP_VERSION_*` in
  `isotp.h`.
- **Rationale:** these are the public names of protocol values and configuration. They
  are used by applications; `ISOTP_N_TIMEOUT_A` is kept so that the N_Result set of
  ISO 15765-2 is complete even though the library writes synchronously and never produces
  it.

### DEV-03: external functions referenced in one translation unit (Rule 8.7, advisory)

- **Location:** the public API functions that the library itself does not call, for
  example `isotp_link_tx_busy()`, `isotp_link_invalid_frames()` and the codec functions
  that only the link calls.
- **Rationale:** they are the library interface and are called by applications. Giving
  them internal linkage would remove them from the API.
