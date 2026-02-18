# Python C API

## How to use

This example shows how to build and use the ISO-TP-C Python C extension module
from the `example/python_api` directory.

## Requirements

- C99 compiler and CMake >= 3.20.
- Python 3 with development headers (Python3::Python).
- A build toolchain supported by CMake (e.g. MSVC, Ninja, Make).

## Build

From the `example/python_api` directory:

```bash
cd example/python_api
cmake -G "Unix Makefiles" -B build
cmake --build build
```

The build outputs a Python extension module:

- Windows: `build/isotp_python_api.pyd`
- Linux: `build/isotp_python_api.so` (or the platform-specific Python extension suffix)

## Use

Start Python from the same directory (or add the build folder to `PYTHONPATH`) and
import the module:

```bash
python -c "import sys; sys.path.insert(0, 'build'); import pyisotp; print(pyisotp)"
```

Basic usage (initialize, send, poll, receive):

```python
import sys
sys.path.insert(0, "build")
import pyisotp

# Parameters: send_id, tx_buf_size, rx_buf_size
link = pyisotp.init(0x123, 4095, 4095)

# Send data
pyisotp.send(link, b"\x01\x02\x03\x04")

# Poll the link (for timeouts/state updates)
pyisotp.poll(link)

# Receive data (returns bytes or None)
payload = pyisotp.receive(link, 4095)
print(payload)
```

### Mock helpers

The module exposes mock utilities used by tests:

- `pyisotp.mock_enable_drop(enable)` - enable or disable frame dropping.
- `pyisotp.time_advance(ms)` - advance virtual time in milliseconds.
- `pyisotp.time_set(ms)` - set virtual time in milliseconds.
- `pyisotp.time_reset()` - reset virtual time to zero.

## Test

Integration tests are optional and use the Python module. Configure CMake with
`ISOTP_INTEGRATION_TESTS=ON`, then build and run the tests from the build tree.

Example:

```bash
cd example/python_api
cmake -G "Unix Makefiles" -B build -DISOTP_INTEGRATION_TESTS=ON
cmake --build build --target integration_tests
```

If your test runner does not find the module, ensure the build output folder is
on `PYTHONPATH` or run tests from the build directory.
