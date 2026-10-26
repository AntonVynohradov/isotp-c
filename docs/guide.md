# Getting Started

This chapter shows how to get ISOTP-C, add it to your project, and build and test it. The
library itself is two C files and five headers; everything else in the repository is
tests, examples, tooling and documentation.

## Get the library

```bash
git clone https://github.com/AntonVynohradov/isotp-c.git
```

Each [release](https://github.com/AntonVynohradov/isotp-c/releases) provides the source
code, a prebuilt static library for Linux x86-64 with its headers, the MISRA report and the
PDF user manual.

## Add it to your project

**CMake.**

```cmake
include(path/to/isotp-c/isotp.cmake)
target_link_libraries(app PRIVATE isotp)
```

**Any other build system.** Compile `src/*.c` and add the repository root and `inc/` to the
include path.

**Prebuilt package.** Link `lib/libisotp.a` and add `include/` to the include path.

## Prerequisites for development

- C11 compiler, CMake 3.20+
- Optional: GoogleTest and gcovr, Python 3 and pytest, Cppcheck, clang-format, MkDocs
  (requirements-dev.txt), Doxygen, WeasyPrint with Pango

## Build with presets

| Preset | Purpose |
| --- | --- |
| `ISOTP[Debug]` | Static library with debug information |
| `ISOTP[Release]` | Optimized static library |
| `ISOTP[UnitTests]` | GoogleTest executable with coverage; target `coverage` |
| `ISOTP[IntegrationTests]` | pytest suite; target `integration_tests` |
| `ISOTP[Examples]` | Host examples and the bare-metal simulation test |

```bash
cmake --preset "ISOTP[Release]"
cmake --build "build/ISOTP[Release]"
```

## Unit tests and coverage

```bash
cmake --preset "ISOTP[UnitTests]"
cmake --build "build/ISOTP[UnitTests]" --target coverage
```

82 GoogleTest tests in `tests/unit/` cover the codec (every frame type, every rejection,
every capacity boundary), STmin, and the link (transmitter, receiver, timeouts, flow
control, errors, callbacks). The unit-test build uses 5000-byte buffers so that the
32-bit FF_DL paths are reachable. The `coverage` target runs them and fails below
`ISOTP_COVERAGE_MIN` (100 %) line or branch coverage of `src/`; the HTML report is in
`build/ISOTP[UnitTests]/Coverage/`.

## Integration tests

```bash
cmake --preset "ISOTP[IntegrationTests]"
cmake --build "build/ISOTP[IntegrationTests]" --target integration_tests
# or directly:
python -m pytest tests/integration -v
```

`tests/integration/isotp/protocol.py` is a second implementation of the frame codec in
Python, written from the standard and using only the standard library (the *Python
twin*). The suite

- checks the reference frames and the frames a receiver must ignore on the twin,
- compiles `src/isotp_protocol.c` with `gcc` and compares it with the twin on 300 random
  messages per encoder, 2 000 random frames for the decoder and every STmin value,
- compiles the whole library with `tests/integration/isotp_harness.c` and lets a peer
  built on the twin exchange messages with the C link on a simulated clock: message sizes
  from 1 to 4999 bytes, block sizes 1 to 255, four STmin values, timeouts, Wait frames,
  overflow, wrong sequence numbers, padding, functional addressing, a busy driver and
  line noise.

The tests that need `gcc` are skipped (and reported as skipped) when it is missing.

## Static analysis

| Target | Tool |
| --- | --- |
| `cppcheck-analyze` | Cppcheck, all checks |
| `misra-analyze` | Cppcheck MISRA C:2012 addon; fails on any violation |
| `codeql-analyze` | CodeQL `cpp-queries` |

## Formatting

```bash
clang-format -i src/*.c inc/*.h isotp.h tests/unit/*.cpp examples/*/*.[ch]
```

## Versioning and releases

The version comes from the latest `vX.Y.Z` Git tag (`cmake/version.cmake`) and is passed to
the compiler as `ISOTP_VERSION_MAJOR/MINOR/PATCH` and `ISOTP_VERSION`; `isotp.h` has
fallbacks for builds without CMake. The project follows Semantic Versioning and keeps a
[changelog](changelog.md).

A release is made from `main` by pushing a tag `vX.Y.Z`. The *Release* workflow checks that
the tag matches `isotp.h`, `cmake/version.cmake` and `CHANGELOG.md`, runs every quality
gate, and publishes `isotp-c-X.Y.Z-linux-x86_64.tar.gz` (static library, headers,
license, readme, changelog), the MISRA report and the PDF manual as a GitHub Release.

## Documentation

| Task | Command | VS Code task |
| --- | --- | --- |
| Live preview | `mkdocs serve` | *Docs: Serve on localhost* |
| Build the website | `mkdocs build` | *Docs: Build site* |
| Build the PDF manual | `python tools/docs/build_manual.py` | *Docs: Build PDF manual* |
| Regenerate diagrams | `python tools/docs/diagrams.py` | *Docs: Generate diagrams* |
| Compile the C snippets of these pages | `python tools/docs/check_snippets.py` | *Docs: Check C snippets* |
