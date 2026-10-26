---
name: isotp-c
description: "Use when working on the isotp ISO 15765-2 (ISO-TP) library: the frame codec, the transport link (segmentation, flow control, timing), the Python twin, their tests, the examples (bare metal, Zephyr, Linux SocketCAN), MISRA C:2012 compliance, releases, or integrating the library into firmware or host tools."
---

# isotp: ISO 15765-2 (ISO-TP) library

## Scope

This repository is a portable C11 static library plus a Python twin of the
codec. The 2.0 API (`struct isotp_link`, explicit time input) replaces the
1.x API of the upstream fork; since the 2.0.0 release the public API in `isotp.h`
and `inc/` follows Semantic Versioning. It implements the
ISO 15765-2 (ISO-TP) transport protocol for classical CAN with normal addressing
only: frame codec and a transport-independent link. Diagnostic services (UDS),
CAN drivers, threads and routing of CAN IDs belong to the applications that use
it, not here.

| Part | Files | Rule |
| --- | --- | --- |
| Umbrella | `isotp.h` | Includes all public headers; fallback `ISOTP_VERSION_*` |
| Configuration | `inc/isotp_config.h` | Build-time sizes, each overridable with `#ifndef` |
| Codec | `inc/isotp_protocol.h`, `src/isotp_protocol.c` | Decode one frame, encode SF/FF/CF/FC, padding, STmin; pure functions |
| Constants | `inc/isotp_defs.h` | Values only from the specification |
| Link | `inc/isotp_link.h`, `src/isotp_link.c` | No OS calls, no clock: time is an argument (`now_us`); I/O only through the write callback |
| Python | `tests/integration/isotp/protocol.py` | Standard library only; byte-identical to the C codec; `test_link.py` drives the C link against it |
| Build | `CMakeLists.txt`, `isotp.cmake`, `cmake/`, `CMakePresets.json` | Library target in `isotp.cmake`; tests and analysis in `cmake/` |
| Examples | `examples/bare_metal`, `examples/linux_socketcan`, `examples/zephyr` | Not library code; built with `ISOTP_EXAMPLES=ON` (Zephyr with west) |

## Hard rules

- The library sources include only `<stddef.h>`, `<stdint.h>`, `<stdbool.h>`,
  `<string.h>` and ISOTP headers. No heap, no RTOS, no driver headers, no global
  mutable state (all state lives in caller-owned structs).
- Public symbols use the `isotp_` / `ISOTP_` prefix. Public headers keep the
  `extern "C"` guards and Doxygen comments for every public item.
- Keep the public API stable within a major version. Record every user-visible
  change under `[Unreleased]` in `CHANGELOG.md`.
- The version comes from the `vX.Y.Z` Git tag (`cmake/version.cmake`); the
  fallbacks `ISOTP_DEFAULT_VERSION` in `cmake/version.cmake` and
  `ISOTP_VERSION_*` / `ISOTP_VERSION` in `isotp.h` equal the latest release.
- MISRA C:2012: `misra-analyze` must report 0 violations. An unavoidable
  violation needs an entry in the deviation register in `docs/misra.md`. Library
  functions have a single `return` at the end, parenthesize mixed-operator
  expressions, cast ignored return values to `(void)`, keep `++`/`--` as separate
  statements, and index arrays instead of doing pointer arithmetic.
- Every public function validates its pointer arguments and lengths (Dir 4.11).
  Do not add code that cannot be reached; prove bounds from an invariant and
  document it in a comment instead.
- Behavior is defined by the specification (https://www.iso.org/standard/84211.html). Do not invent values.
- Code must compile without warnings with
  `-std=c11 -Wall -Wextra -Wconversion -Wpedantic`.
- Style: `.clang-format` (4 spaces, Allman braces, 100 columns); file banners and
  section comments as in the existing files (`.vscode/cpp.json.code-snippets`).

## Tests (definition of done)

```sh
cmake --preset "ISOTP[Debug]" && cmake --build "build/ISOTP[Debug]"
cmake --preset "ISOTP[UnitTests]" && cmake --build "build/ISOTP[UnitTests]" --target coverage
cmake --preset "ISOTP[IntegrationTests]" && cmake --build "build/ISOTP[IntegrationTests]" --target integration_tests
cmake --build "build/ISOTP[Debug]" --target misra-analyze
cmake --preset "ISOTP[Examples]" && cmake --build "build/ISOTP[Examples]" && ctest --test-dir "build/ISOTP[Examples]" -L example
python tools/docs/check_snippets.py
```

The `coverage` target fails below 100 % line or branch coverage of `src/`;
`misra-analyze` fails on any violation. Unit tests are GoogleTest files
`tests/unit/gtest_isotp_*.cpp` (register new files in `isotp.cmake`). The
pytest suite cross-checks the C codec against the Python twin; it is skipped
without `gcc`, so say so when reporting results on such a machine.

Reference vectors that must keep passing (built from the frame layout of
ISO 15765-2, normal addressing, classical CAN): SF `02 10 03`; FF `10 14 00 01 02 03
04 05` (20 bytes); FF with 32-bit FF_DL `10 00 00 00 10 00 AA BB` (4096 bytes);
CF `21 06 07 08`; FC `30 08 14` (CTS, BS 8, STmin 20 ms); reserved STmin values
decode as 127 ms. Unit tests build with 5000-byte buffers so that the 32-bit FF_DL
paths are reachable; keep `ISOTP_TX/RX_BUFFER_SIZE` in `isotp.cmake` above 4095.

## Documentation

MkDocs Material website (`mkdocs.yml`, pages in `docs/`) and a printable PDF manual
built from the same pages (`python tools/docs/build_manual.py`). All documentation
is in English. The documentation tooling is the shared base of the
`sfs-protocol-library` skill: keep `overrides/`, `tools/docs/build_manual.py`,
`manual.css`, `svgkit.py`, `check_snippets.py`, `stack_usage.py` and the stylesheet
identical to the skill's templates and change project data, not structure
(`mkdocs.yml → extra.landing / extra.manual`, `tools/docs/diagrams.py`,
`tools/docs/snippets.json`). Edit the diagram scripts and run them; never edit the
SVG files.

## Releases

1. Set `ISOTP_DEFAULT_VERSION` in `cmake/version.cmake` and the fallbacks in
   `isotp.h` to the new version.
2. Move the `[Unreleased]` entries in `CHANGELOG.md` to `[X.Y.Z] - date`.
3. Merge to `main`, then push the tag `vX.Y.Z`. The Release workflow refuses a tag
   that does not match these files, runs all quality gates and publishes the
   library, headers, MISRA report and PDF manual as a GitHub Release.

Never push a tag or publish a release without the maintainer's explicit request.
