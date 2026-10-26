#******************************************************************************
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Python twin tests and the C/Python codec cross-check.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#******************************************************************************
"""Frame codec: reference vectors in the Python twin, then C against Python.

Run from the repository root:   python -m pytest tests/integration -v
"""

from __future__ import annotations

import ctypes
import random

import pytest

from conftest import CPdu
from isotp import protocol as p

# Frames built from the frame layout of ISO 15765-2 (normal addressing, classical CAN).
VECTORS = [
    # (frame, type, message_length, payload)
    ("02 10 03", p.SF, 2, "10 03"),                     # UDS DiagnosticSessionControl
    ("07 00 01 02 03 04 05 06", p.SF, 7, "00 01 02 03 04 05 06"),
    ("10 14 00 01 02 03 04 05", p.FF, 20, "00 01 02 03 04 05"),
    ("1F FF 00 01 02 03 04 05", p.FF, 4095, "00 01 02 03 04 05"),
    ("10 00 00 00 10 00 AA BB", p.FF, 4096, "AA BB"),   # escape sequence, 32-bit FF_DL
]


@pytest.mark.parametrize("frame,kind,length,payload", VECTORS)
def test_reference_vectors(frame, kind, length, payload):
    pdu = p.decode(bytes.fromhex(frame))
    assert pdu is not None
    assert (pdu.type, pdu.message_length, pdu.payload) == (kind, length, bytes.fromhex(payload))


def test_consecutive_and_flow_control_vectors():
    cf = p.decode(bytes.fromhex("21 06 07 08"))
    assert (cf.type, cf.sn, cf.payload) == (p.CF, 1, bytes.fromhex("06 07 08"))
    fc = p.decode(bytes.fromhex("30 08 14"))
    assert (fc.type, fc.fs, fc.bs, fc.st_min) == (p.FC, p.FS_CTS, 8, 0x14)


@pytest.mark.parametrize("frame", ["00 AA", "03 AA BB", "10 07 00 01 02 03 04 05",
                                   "10 14 00 01 02", "10 00 00 00 0F FF 00 01", "21",
                                   "30 00", "40 00", "F0 00 00", ""])
def test_frames_a_receiver_ignores(frame):
    assert p.decode(bytes.fromhex(frame)) is None


def test_st_min_round_trip_never_faster():
    for us in range(0, 130001, 37):
        assert p.st_min_decode(p.st_min_encode(us)) >= min(us, 127000)
    assert [p.st_min_decode(v) for v in (0x80, 0xF0, 0xFA, 0xFF)] == [127000] * 4


def test_segment_round_trip():
    for n in (8, 9, 62, 4095, 4096, 4999):
        message = bytes(random.Random(n).randrange(256) for _ in range(n))
        frames = p.segment(message)
        first = p.decode(frames[0])
        data = first.payload
        for i, frame in enumerate(frames[1:], start=1):
            cf = p.decode(frame)
            assert cf.sn == i & 0x0F
            data += cf.payload
        assert data[:first.message_length] == message


# ------------------------------------------------------------------ C cross-check

def c_encode(lib, function, *args):
    out = ctypes.create_string_buffer(8)
    n = function(out, 8, *args)
    return out.raw[:n] if n > 0 else None


def c_decode(lib, frame: bytes):
    pdu = CPdu()
    if lib.isotp_decode(frame, len(frame), ctypes.byref(pdu)) != 0:
        return None
    payload = frame[pdu.data_offset:pdu.data_offset + pdu.data_length]
    return p.Pdu(pdu.type, pdu.message_length, payload, pdu.sequence_number, pdu.flow_status,
                 pdu.block_size, pdu.st_min)


def test_c_encoders_match_twin(codec_lib):
    rng = random.Random(15765)
    for _ in range(300):
        data = bytes(rng.randrange(256) for _ in range(7))
        n = rng.randint(1, 7)
        assert c_encode(codec_lib, codec_lib.isotp_encode_single, data[:n], n) == \
            p.encode_single(data[:n])
        sn = rng.randrange(16)
        assert c_encode(codec_lib, codec_lib.isotp_encode_consecutive, sn, data[:n], n) == \
            p.encode_consecutive(sn, data[:n])
        fs, bs, st = rng.randrange(3), rng.randrange(256), rng.randrange(256)
        assert c_encode(codec_lib, codec_lib.isotp_encode_flow_control, fs, bs, st) == \
            p.encode_flow_control(fs, bs, st)


def test_c_first_frame_matches_twin(codec_lib):
    rng = random.Random(4095)
    for _ in range(300):
        length = rng.choice([8, rng.randint(9, 4095), rng.randint(4096, 0xFFFFFFFF)])
        head = bytes(rng.randrange(256) for _ in range(6))
        c_frame = c_encode(codec_lib, codec_lib.isotp_encode_first, head, length)
        if length <= 4095:
            # the twin builds a First Frame from the start of the message
            assert c_frame == p.encode_first(head + bytes(length - 6))
        else:
            assert c_frame == bytes([0x10, 0x00]) + length.to_bytes(4, "big") + head[:2]
        assert p.decode(c_frame).message_length == length


def test_c_decoder_matches_twin(codec_lib):
    rng = random.Random(2016)
    for _ in range(2000):
        n = rng.randint(0, 9)
        frame = bytes(rng.randrange(256) for _ in range(n))
        if n and rng.random() < 0.7:  # bias towards the four valid frame types
            frame = bytes([(rng.randrange(4) << 4) | (frame[0] & 0x0F)]) + frame[1:]
        assert c_decode(codec_lib, frame) == p.decode(frame), frame.hex(" ")


def test_c_st_min_matches_twin(codec_lib):
    for value in range(256):
        assert codec_lib.isotp_st_min_decode(value) == p.st_min_decode(value)
    for us in list(range(0, 2001)) + list(range(2001, 140000, 997)) + [0xFFFFFFFF]:
        assert codec_lib.isotp_st_min_encode(us) == p.st_min_encode(us), us
