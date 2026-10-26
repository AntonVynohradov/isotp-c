#******************************************************************************
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Isometric hero scene of ISOTP-C: a long message split into flow-controlled CAN frames.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#******************************************************************************
"""Hero image of ISOTP-C, built on svgkit.Iso (the shared toolkit stays generic).

The picture shows the mechanism that sets ISO-TP apart from the other libraries of
the family: a message far longer than a CAN frame leaves the chip as a First Frame,
the receiving ECU answers with a Flow Control frame, the Consecutive Frames follow
at the pace it asked for, and the message is reassembled cell by cell at the ECU.
Malformed frames (red) fall off the bus and are ignored.
"""

from __future__ import annotations

from typing import List, Tuple

from svgkit import HERO_DEFS, HERO_STYLE, Iso, _write

LOOP = 6.0  # seconds


def _times(*points: float) -> str:
    return ";".join(f"{p / LOOP:.4f}" for p in points)


def _packet(iso: Iso, track: str, depart: float, travel: float, color: str, glow: str,
            label: str, width: float = 18.0) -> None:
    """One frame moving along track, visible only while it travels."""
    arrive = depart + travel
    body = (f'<rect x="{-width / 2:.1f}" y="-6" width="{width:.1f}" height="12" rx="2.5" '
            f'fill="{color}" stroke="{glow}" stroke-width="0.9" transform="skewY(30)"/>'
            f'<text x="0" y="3.6" transform="skewY(30)" text-anchor="middle" '
            f'font-family="JetBrains Mono, monospace" font-size="8.5" font-weight="700" '
            f'fill="#050b16">{label}</text>')
    iso.parts.append(
        f'<g opacity="0" filter="url(#glow)">{body}'
        f'<animateMotion dur="{LOOP}s" repeatCount="indefinite" path="{track}" '
        f'keyPoints="0;0;1;1" keyTimes="{_times(0, depart, arrive, LOOP)}" calcMode="linear"/>'
        f'<animate attributeName="opacity" dur="{LOOP}s" repeatCount="indefinite" '
        f'values="0;0;1;1;0;0" '
        f'keyTimes="{_times(0, depart, depart + 0.05, arrive, arrive + 0.05, LOOP)}"/></g>')


def isotp_hero(path: str) -> None:
    iso = Iso()
    p = iso.p

    # --- board with the library chip, the CAN transceiver and the connector
    iso.box(0, 10, 0, 7.2, -0.4, 0, "url(#boardTop)", "#0a1628", "#071122", "#1e3a66", 1.2)
    traces = []
    for i in range(6):
        y = 2.3 + i * 0.55
        traces.append([p(2.0, y), p(1.1, y), p(0.7, y - 0.4), p(0.25, y - 0.4)])
        x = 2.3 + i * 0.55
        traces.append([p(x, 2.0), p(x, 1.1), p(x + 0.35, 0.75), p(x + 0.35, 0.25)])
    for trace in traces:
        iso.path(trace, "#1d5fbf", 1.3, ' opacity="0.7"')
    for trace in ([p(5.4, 3.6), p(6.4, 3.6), p(6.9, 4.1), p(7.4, 4.1)],
                  [p(8.7, 4.1), p(8.9, 4.1), p(8.9, 5.6), p(9.2, 5.9)]):
        iso.path(trace, "#1d5fbf", 1.6)
        iso.path(trace, "#5ea6ff", 1.6, ' class="flow"')
    iso.box(7.4, 8.7, 3.4, 4.8, 0, 0.45, "#14243c", "#0c182b", "#09121f", "#2a4870")
    iso.text(8.05, 4.1, 0.46, "CAN", 0.34, "#6b7d99", 600)
    iso.box(9.2, 10.0, 5.3, 6.5, 0, 0.55, "#c9d6ea", "#8aa0bf", "#6f86a6")
    for i in range(7):
        c = 2.25 + i * 0.45
        iso.box(c - 0.08, c + 0.08, 5.2, 5.45, 0, 0.12, "#8fa6c6", "#6b819f", "#5a6f8b")
        iso.box(5.2, 5.45, c - 0.08, c + 0.08, 0, 0.12, "#8fa6c6", "#6b819f", "#5a6f8b")
    iso.box(2.0, 5.2, 2.0, 5.2, 0, 0.75, "url(#chipTop)", "#0a1322", "#060d18", "#2f8cff", 1.0)
    iso.text(3.6, 3.35, 0.76, "ISOTP-C", 0.5, "#e6edf7", 800, 0.03)
    iso.text(3.6, 3.95, 0.76, "ISO 15765-2", 0.24, "#5ea6ff", 600, 0.06)
    iso.led(2.45, 2.45, 0.76)

    # --- the long message on the chip: six cells, sent as FF + CFs
    for i in range(6):
        x0 = 2.35 + i * 0.47
        iso.box(x0, x0 + 0.4, 4.45, 4.85, 0.76, 0.86, "#2f8cff", "#1a6fe0", "#1559b8")

    # --- ECUs on the bus; the middle one receives
    xs = [13.0 + i * 4.4 for i in range(3)]
    names = ["ECU 7E8", "ECU 7E9", "ECU 7EA"]
    target = 1
    for i, (x0, name) in enumerate(zip(xs, names)):
        iso.box(x0, x0 + 2.6, 3.0, 5.4, -0.4, 1.3, "#1b2a40", "#131f31", "#0e1726", "#2a4870")
        iso.box(x0 + 0.35, x0 + 2.25, 3.4, 3.8, 1.3, 1.45, "#c9d6ea", "#8aa0bf", "#6f86a6")
        iso.text(x0 + 1.3, 5.4, 0.3, name, 0.38, "#9aabc4", 700, face="y")
        iso.led(x0 + 2.2, 5.4, 1.0, "led-rx" if i == target else "led", r=2.6)

    # reassembly slots on the receiving ECU: light up as the frames arrive
    arrivals = [0.8, 2.4, 2.9, 3.4, 3.9, 4.4]
    x0 = xs[target]
    for i, at in enumerate(arrivals):
        cx = x0 + 0.35 + i * 0.33
        cell = []
        for (x, y) in ((cx, 4.3), (cx + 0.28, 4.3), (cx + 0.28, 4.75), (cx, 4.75)):
            cell.append(p(x, y, 1.31))
        pts = " ".join(f"{a:.1f},{b:.1f}" for a, b in cell)
        iso.parts.append(
            f'<polygon points="{pts}" fill="#3fcf8e" opacity="0.12">'
            f'<animate attributeName="opacity" dur="{LOOP}s" repeatCount="indefinite" '
            f'values="0.12;0.12;1;1;0.12" keyTimes="{_times(0, at, at + 0.05, 5.6, LOOP)}"/>'
            f'</polygon>')

    # --- the CAN bus (CAN_H / CAN_L) and the drops
    bus_y, bus_z = 6.6, -0.35
    end_x = xs[-1] + 3.4
    for dy, color in ((-0.12, "#2f8cff"), (0.12, "#1e5aa8")):
        bus = [p(10.0, 5.9 + dy, -0.2), p(10.6, bus_y + dy, bus_z), p(end_x, bus_y + dy, bus_z)]
        iso.path(bus, "#0e2a4f", 6)
        iso.path(bus, color, 2.0, ' opacity="0.9"')
    iso.path([p(10.6, bus_y, bus_z), p(end_x, bus_y, bus_z)], "#9cc8ff", 2.0,
             ' class="flow-bus"')
    for x0 in xs:
        drop = [p(x0 + 1.3, bus_y, bus_z), p(x0 + 1.3, 5.4, bus_z), p(x0 + 1.3, 5.4, 0.1)]
        iso.path(drop, "#0e2a4f", 5)
        iso.path(drop, "#2f8cff", 1.6, ' opacity="0.8"')

    # malformed frames fall off the line: ignored by the receiver
    for i, (nx, delay) in enumerate(((11.6, 0.4), (12.2, 1.6), (11.9, 2.6))):
        sx, sy = p(nx, bus_y, bus_z + 0.25)
        iso.parts.append(f'<g transform="translate({sx:.1f} {sy:.1f})"><g class="noise" '
                         f'style="animation-delay:{delay}s"><rect x="-3.5" y="-3.5" width="7" '
                         f'height="7" rx="1.2" fill="#ff5c6c" transform="rotate({20 + 25 * i})"/>'
                         f'</g></g>')

    # --- frames: FF out, FC back, CFs out at the separation time the ECU asked for
    start = p(10.6, bus_y, bus_z + 0.2)
    stop = p(xs[target] + 1.3, bus_y, bus_z + 0.2)
    out_track = f"M{start[0]:.1f},{start[1]:.1f} L{stop[0]:.1f},{stop[1]:.1f}"
    back_track = f"M{stop[0]:.1f},{stop[1]:.1f} L{start[0]:.1f},{start[1]:.1f}"
    _packet(iso, out_track, 0.0, 0.8, "#2f8cff", "#9cc8ff", "FF", 22)
    _packet(iso, back_track, 0.95, 0.75, "#e07a00", "#ffc27a", "FC", 22)
    for k, depart in enumerate((1.7, 2.2, 2.7, 3.2, 3.7)):
        _packet(iso, out_track, depart, 0.7, "#5ea6ff", "#cfe4ff", f"CF{k + 1}", 24)

    label = ("ISOTP-C splits a long message into CAN frames: First Frame out, "
             "Flow Control back, Consecutive Frames at the receiver's pace")
    _write(path, iso.svg(label, HERO_DEFS, HERO_STYLE, glow_at=(12, 5)))
