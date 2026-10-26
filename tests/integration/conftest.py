#******************************************************************************
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Pytest fixtures: build the C library with gcc and load it with ctypes.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#******************************************************************************
"""Shared fixtures of the integration tests.

The C sources are compiled with the host gcc into a shared library and loaded
with ctypes; tests that need it are skipped (with the reason shown) when gcc is
not installed. The Python twin in isotp/protocol.py needs nothing but Python.
"""

from __future__ import annotations

import ctypes
import os
import shutil
import subprocess
import sys
import tempfile

import pytest

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, "..", ".."))
sys.path.insert(0, HERE)

BUFFER_SIZE = 5000  # larger than 4095, so the 32-bit FF_DL paths are exercised


def _build(name: str, sources: list[str]) -> ctypes.CDLL:
    compiler = shutil.which("gcc") or shutil.which("cc")
    if compiler is None:
        pytest.skip("gcc not found: C/Python cross-check skipped")
    out_dir = tempfile.mkdtemp(prefix="isotp-it-")
    suffix = ".dll" if sys.platform == "win32" else ".so"
    library = os.path.join(out_dir, name + suffix)
    command = [compiler, "-std=c11", "-O1", "-shared", "-fPIC", "-Wall", "-Wextra",
               f"-DISOTP_TX_BUFFER_SIZE={BUFFER_SIZE}U", f"-DISOTP_RX_BUFFER_SIZE={BUFFER_SIZE}U",
               "-I", ROOT, "-I", os.path.join(ROOT, "inc"), "-o", library]
    command += [os.path.join(ROOT, s) for s in sources]
    subprocess.run(command, check=True)
    return ctypes.CDLL(library)


@pytest.fixture(scope="session")
def codec_lib() -> ctypes.CDLL:
    """src/isotp_protocol.c alone."""
    lib = _build("isotp_codec", ["src/isotp_protocol.c"])
    lib.isotp_encode_single.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p,
                                        ctypes.c_uint32]
    lib.isotp_encode_first.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p,
                                       ctypes.c_uint32]
    lib.isotp_encode_consecutive.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_uint8,
                                             ctypes.c_char_p, ctypes.c_uint32]
    lib.isotp_encode_flow_control.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_uint8,
                                              ctypes.c_uint8, ctypes.c_uint8]
    lib.isotp_decode.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_void_p]
    lib.isotp_st_min_encode.argtypes = [ctypes.c_uint32]
    lib.isotp_st_min_encode.restype = ctypes.c_uint8
    lib.isotp_st_min_decode.argtypes = [ctypes.c_uint8]
    lib.isotp_st_min_decode.restype = ctypes.c_uint32
    return lib


class CPdu(ctypes.Structure):
    """Mirror of struct isotp_pdu."""

    _fields_ = [("type", ctypes.c_uint8), ("message_length", ctypes.c_uint32),
                ("data_offset", ctypes.c_uint8), ("data_length", ctypes.c_uint8),
                ("sequence_number", ctypes.c_uint8), ("flow_status", ctypes.c_uint8),
                ("block_size", ctypes.c_uint8), ("st_min", ctypes.c_uint8)]


class CLink:
    """One C link driven through tests/integration/isotp_harness.c."""

    def __init__(self, lib: ctypes.CDLL):
        self.lib = lib

    def init(self, tx_id=0x7E8, block_size=0, st_min_us=0, n_bs_us=1_000_000,
             n_cr_us=1_000_000, wft_max=10, padding=False) -> int:
        return self.lib.harness_init(tx_id, block_size, st_min_us, n_bs_us, n_cr_us, wft_max,
                                     int(padding))

    def send(self, data: bytes, now: int = 0) -> int:
        return self.lib.harness_send(data, len(data), now)

    def send_functional(self, can_id: int, data: bytes) -> int:
        return self.lib.harness_send_functional(can_id, data, len(data))

    def receive(self, frame: bytes, now: int = 0) -> None:
        self.lib.harness_receive(frame, len(frame), now)

    def poll(self, now: int = 0) -> None:
        self.lib.harness_poll(now)

    def fail_writes(self, code: int, count: int) -> None:
        self.lib.harness_fail_writes(code, count)

    def frames(self) -> list[tuple[int, bytes]]:
        out = []
        buf = ctypes.create_string_buffer(8)
        can_id = ctypes.c_uint32()
        for i in range(self.lib.harness_frames_written()):
            n = self.lib.harness_frame(i, buf, ctypes.byref(can_id))
            out.append((can_id.value, buf.raw[:n]))
        return out

    def messages(self) -> int:
        return self.lib.harness_messages()

    def last_message(self) -> bytes:
        buf = ctypes.create_string_buffer(BUFFER_SIZE)
        n = self.lib.harness_last_message(buf, BUFFER_SIZE)
        return buf.raw[:n]

    def tx_results(self) -> list[int]:
        arr = (ctypes.c_int * 64)()
        n = self.lib.harness_tx_results(arr, 64)
        return list(arr[:n])

    def rx_errors(self) -> list[int]:
        arr = (ctypes.c_int * 64)()
        n = self.lib.harness_rx_errors(arr, 64)
        return list(arr[:n])

    def tx_busy(self) -> bool:
        return bool(self.lib.harness_tx_busy())

    def invalid_frames(self) -> int:
        return self.lib.harness_invalid_frames()


@pytest.fixture(scope="session")
def harness_lib() -> ctypes.CDLL:
    lib = _build("isotp_harness", ["src/isotp_protocol.c", "src/isotp_link.c",
                                   "tests/integration/isotp_harness.c"])
    lib.harness_send.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_uint32]
    lib.harness_send_functional.argtypes = [ctypes.c_uint32, ctypes.c_char_p, ctypes.c_uint32]
    lib.harness_receive.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_uint32]
    lib.harness_poll.argtypes = [ctypes.c_uint32]
    lib.harness_init.argtypes = [ctypes.c_uint32, ctypes.c_uint8, ctypes.c_uint32,
                                 ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint8, ctypes.c_int]
    lib.harness_last_message.restype = ctypes.c_uint32
    lib.harness_invalid_frames.restype = ctypes.c_uint32
    lib.harness_fail_writes.argtypes = [ctypes.c_int, ctypes.c_uint]
    return lib


@pytest.fixture
def link(harness_lib) -> CLink:
    """A freshly initialized C link (tx_id 0x7E8, BS 0, STmin 0, 1 s timeouts)."""
    c_link = CLink(harness_lib)
    assert c_link.init() == 0
    return c_link
