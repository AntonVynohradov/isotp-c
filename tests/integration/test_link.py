#******************************************************************************
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: End-to-end tests: the C link against a peer built on the Python twin.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#******************************************************************************
"""The C link (through isotp_harness.c) exchanges messages with a peer written
on the Python twin, on a simulated clock, so both directions of ISO 15765-2 are
checked against an independent implementation: segmentation, flow control,
separation time, timeouts and error handling.
"""

from __future__ import annotations

import random

import pytest

from isotp import protocol as p

N_OK, N_TIMEOUT_BS, N_TIMEOUT_CR, N_WRONG_SN = 0, -2, -3, -4
N_INVALID_FS, N_UNEXP_PDU, N_WFT_OVRN, N_BUFFER_OVFLW, N_ERROR = -5, -6, -7, -8, -9
ISOTP_BUSY = -2

SIZES = [1, 2, 7, 8, 9, 62, 63, 4095, 4096, 4999]


def message(n: int, seed: int = 0) -> bytes:
    rng = random.Random(n * 31 + seed)
    return bytes(rng.randrange(256) for _ in range(n))


def peer_send(link, data: bytes, now: int = 0) -> list[bytes]:
    """The twin sends data to the C link, obeying the Flow Control the link returns."""
    if len(data) <= 7:
        link.receive(p.encode_single(data), now)
        return []
    base = len(link.frames())
    frames = p.segment(data)
    link.receive(frames[0], now)
    flow = []
    remaining = 0
    for frame in frames[1:]:
        if remaining == 0:
            fc = p.decode(link.frames()[base + len(flow)][1])
            assert fc.type == p.FC and fc.fs == p.FS_CTS
            flow.append(fc)
            remaining = fc.bs if fc.bs else 1 << 30
        link.receive(frame, now)
        remaining -= 1
    return [link.frames()[base + i][1] for i in range(len(flow))]


def peer_receive(link, n: int, bs: int = 0, st_min: int = 0, step_us: int = 100,
                 now: int = 0, limit: int = 100000):
    """The twin receives what the C link sends; returns (message, CF send times)."""
    first = p.decode(link.frames()[0][1])
    assert first.type == p.FF and first.message_length == n
    data = bytearray(first.payload)
    link.receive(p.encode_flow_control(p.FS_CTS, bs, st_min), now)
    seen, times, in_block, sn = 1, [], 0, 1
    for _ in range(limit):
        if not link.tx_busy():
            break
        now += step_us
        link.poll(now)
        frames = link.frames()
        while seen < len(frames):
            cf = p.decode(frames[seen][1])
            seen += 1
            assert cf.type == p.CF and cf.sn == sn
            sn = (sn + 1) & 0x0F
            data += cf.payload
            times.append(now)
            in_block += 1
            if bs and in_block == bs and link.tx_busy():
                in_block = 0
                link.receive(p.encode_flow_control(p.FS_CTS, bs, st_min), now)
    return bytes(data[:n]), times


@pytest.mark.parametrize("n", SIZES)
def test_c_receives_from_twin(link, n):
    data = message(n)
    peer_send(link, data)
    assert link.messages() == 1
    assert link.last_message() == data
    assert link.rx_errors() == []


@pytest.mark.parametrize("n", SIZES)
def test_c_sends_to_twin(link, n):
    data = message(n, 1)
    assert link.send(data) == 0
    if n <= 7:
        assert p.decode(link.frames()[0][1]).payload == data
    else:
        received, _ = peer_receive(link, n)
        assert received == data
    assert link.tx_results() == [N_OK]
    assert all(can_id == 0x7E8 for can_id, _ in link.frames())


@pytest.mark.parametrize("bs", [1, 2, 8, 255])
def test_block_sizes_both_directions(harness_lib, link, bs):
    assert link.init(block_size=bs, st_min_us=1000) == 0
    data = message(300, bs)
    flow = peer_send(link, data)
    assert link.last_message() == data
    assert all(p.decode(fc).bs == bs and p.decode(fc).st_min == 0x01 for fc in flow)
    assert len(flow) == -(-42 // bs)  # 300 = FF (6) + 42 CF: one FC per started block

    assert link.init() == 0
    assert link.send(data) == 0
    received, _ = peer_receive(link, 300, bs=bs)
    assert received == data and link.tx_results() == [N_OK]


@pytest.mark.parametrize("st_min,gap_us", [(0x01, 1000), (0x0A, 10000), (0xF3, 300),
                                           (0x80, 127000)])
def test_c_sender_respects_st_min(link, st_min, gap_us):
    data = message(50)
    assert link.send(data) == 0
    received, times = peer_receive(link, 50, st_min=st_min, step_us=50)
    assert received == data
    gaps = [b - a for a, b in zip(times, times[1:])]
    assert gaps and min(gaps) >= gap_us


def test_sender_timeout_without_flow_control(link):
    assert link.init(n_bs_us=1000) == 0
    assert link.send(message(20)) == 0
    link.poll(999)
    assert link.tx_busy()
    link.poll(1000)
    assert link.tx_results() == [N_TIMEOUT_BS]


def test_receiver_timeout_between_frames(link):
    assert link.init(n_cr_us=1000) == 0
    frames = p.segment(message(30))
    link.receive(frames[0], 0)
    link.receive(frames[1], 100)
    link.poll(1099)
    assert link.rx_errors() == []
    link.poll(1100)
    assert link.rx_errors() == [N_TIMEOUT_CR]


def test_wrong_sequence_number(link):
    frames = p.segment(message(30))
    link.receive(frames[0])
    link.receive(frames[2])
    assert link.rx_errors() == [N_WRONG_SN]
    assert link.messages() == 0


def test_wait_frames_then_continue(link):
    assert link.init(wft_max=2) == 0
    data = message(20)
    assert link.send(data) == 0
    link.receive(p.encode_flow_control(p.FS_WAIT, 0, 0))
    link.receive(p.encode_flow_control(p.FS_WAIT, 0, 0))
    received, _ = peer_receive(link, 20)
    assert received == data and link.tx_results() == [N_OK]


def test_wait_frame_overrun(link):
    assert link.init(wft_max=1) == 0
    assert link.send(message(20)) == 0
    link.receive(p.encode_flow_control(p.FS_WAIT, 0, 0))
    link.receive(p.encode_flow_control(p.FS_WAIT, 0, 0))
    assert link.tx_results() == [N_WFT_OVRN]


def test_overflow_both_directions(link):
    too_long = p.encode_first(b"\0" * 6000)  # buffer is 5000 bytes
    link.receive(too_long)
    assert p.decode(link.frames()[0][1]).fs == p.FS_OVFLW
    assert link.rx_errors() == [N_BUFFER_OVFLW]

    assert link.init() == 0
    assert link.send(message(20)) == 0
    link.receive(p.encode_flow_control(p.FS_OVFLW, 0, 0))
    assert link.tx_results() == [N_BUFFER_OVFLW]


def test_invalid_flow_status(link):
    assert link.send(message(20)) == 0
    link.receive(bytes([0x3F, 0x00, 0x00]))
    assert link.tx_results() == [N_INVALID_FS]


def test_new_first_frame_interrupts(link):
    frames = p.segment(message(30))
    link.receive(frames[0])
    link.receive(frames[1])
    data = message(40, 2)
    peer_send(link, data)
    assert link.rx_errors() == [N_UNEXP_PDU]
    assert link.last_message() == data


def test_padding(link):
    assert link.init(padding=True) == 0
    assert link.send(b"\x3e\x00") == 0
    data = message(20)
    peer_send(link, data)
    assert all(len(frame) == 8 for _, frame in link.frames())
    assert link.frames()[0][1] == p.pad(b"\x02\x3e\x00")


def test_functional_single_frame(link):
    assert link.send_functional(0x7DF, b"\x3e\x80") == 0
    assert link.frames() == [(0x7DF, b"\x02\x3e\x80")]
    assert link.send_functional(0x7DF, message(8)) == -1


def test_driver_busy_is_retried(link):
    data = message(30)
    assert link.send(data) == 0
    link.receive(p.encode_flow_control(p.FS_CTS, 0, 0))
    link.fail_writes(ISOTP_BUSY, 3)
    for t in range(1, 4):
        link.poll(t)
    assert len(link.frames()) == 1
    received, _ = peer_receive_after_fc(link, 30)
    assert received == data


def peer_receive_after_fc(link, n):
    """Collect a transmission whose Flow Control was already given."""
    data = bytearray(p.decode(link.frames()[0][1]).payload)
    now = 10
    while link.tx_busy():
        now += 10
        link.poll(now)
    for _, frame in link.frames()[1:]:
        data += p.decode(frame).payload
    return bytes(data[:n]), None


def test_noise_between_frames_is_ignored(link):
    rng = random.Random(7)
    noise = [bytes.fromhex(h) for h in ("00 AA", "03 AA BB", "40 00", "F0", "30 00", "21")]
    data = message(200)
    frames = p.segment(data)
    link.receive(frames[0])
    for frame in frames[1:]:
        link.receive(rng.choice(noise[:4]))  # malformed: counted and ignored
        link.receive(frame)
    assert link.last_message() == data
    assert link.invalid_frames() == len(frames) - 1
