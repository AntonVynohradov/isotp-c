#******************************************************************************
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Independent Python implementation of the ISO 15765-2 frame codec (Python twin).
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#******************************************************************************
"""Python twin of the ISO-TP frame codec (classical CAN, normal addressing).

Written from ISO 15765-2 independently of the C sources, using only the standard
library, so that the integration tests can compare the two implementations.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import List, Optional

SF, FF, CF, FC = 0x0, 0x1, 0x2, 0x3
FS_CTS, FS_WAIT, FS_OVFLW = 0x0, 0x1, 0x2
CAN_DL = 8
SF_MAX = 7
FF_DL_12BIT_MAX = 4095


@dataclass
class Pdu:
    """One decoded frame; payload is the data carried by this frame."""

    type: int
    message_length: int = 0
    payload: bytes = b""
    sn: int = 0
    fs: int = 0
    bs: int = 0
    st_min: int = 0


def decode(frame: bytes) -> Optional[Pdu]:
    """Return the PDU of a valid frame, or None when a receiver must ignore it."""
    if not 1 <= len(frame) <= CAN_DL:
        return None
    kind, low = frame[0] >> 4, frame[0] & 0x0F
    if kind == SF:
        if low == 0 or low > len(frame) - 1:
            return None
        return Pdu(SF, low, bytes(frame[1:1 + low]))
    if kind == FF:
        if len(frame) != CAN_DL:
            return None
        length = (low << 8) | frame[1]
        if length == 0:
            length = int.from_bytes(frame[2:6], "big")
            if length <= FF_DL_12BIT_MAX:
                return None
            return Pdu(FF, length, bytes(frame[6:8]))
        if length <= SF_MAX:
            return None
        return Pdu(FF, length, bytes(frame[2:8]))
    if kind == CF:
        if len(frame) < 2:
            return None
        return Pdu(CF, sn=low, payload=bytes(frame[1:]))
    if kind == FC:
        if len(frame) < 3:
            return None
        return Pdu(FC, fs=low, bs=frame[1], st_min=frame[2])
    return None


def encode_single(payload: bytes) -> bytes:
    if not 1 <= len(payload) <= SF_MAX:
        raise ValueError("Single Frame carries 1..7 bytes")
    return bytes([len(payload)]) + bytes(payload)


def encode_first(message: bytes) -> bytes:
    n = len(message)
    if n <= SF_MAX:
        raise ValueError("a First Frame starts a message of at least 8 bytes")
    if n <= FF_DL_12BIT_MAX:
        return bytes([0x10 | (n >> 8), n & 0xFF]) + bytes(message[:6])
    return bytes([0x10, 0x00]) + n.to_bytes(4, "big") + bytes(message[:2])


def encode_consecutive(sn: int, data: bytes) -> bytes:
    if not 0 <= sn <= 15 or not 1 <= len(data) <= 7:
        raise ValueError("invalid Consecutive Frame")
    return bytes([0x20 | sn]) + bytes(data)


def encode_flow_control(fs: int, bs: int, st_min: int) -> bytes:
    if fs not in (FS_CTS, FS_WAIT, FS_OVFLW):
        raise ValueError("invalid flow status")
    return bytes([0x30 | fs, bs, st_min])


def pad(frame: bytes, value: int = 0xCC) -> bytes:
    return bytes(frame) + bytes([value]) * (CAN_DL - len(frame))


def st_min_encode(us: int) -> int:
    """Separation time to STmin byte, rounded up (never faster than asked)."""
    if us == 0:
        return 0x00
    if us <= 900:
        return 0xF0 + (us + 99) // 100
    if us < 127000:
        return (us + 999) // 1000
    return 0x7F


def st_min_decode(value: int) -> int:
    """STmin byte to microseconds; reserved values mean 127 ms."""
    if value <= 0x7F:
        return value * 1000
    if 0xF1 <= value <= 0xF9:
        return (value - 0xF0) * 100
    return 127000


def segment(message: bytes) -> List[bytes]:
    """FF + CFs of a message (unpadded), SN starting at 1."""
    frames = [encode_first(message)]
    offset = 6 if len(message) <= FF_DL_12BIT_MAX else 2
    sn = 1
    while offset < len(message):
        chunk = message[offset:offset + 7]
        frames.append(encode_consecutive(sn, chunk))
        offset += len(chunk)
        sn = (sn + 1) & 0x0F
    return frames
