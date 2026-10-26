#******************************************************************************
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Description: Generates the SVG diagrams of the documentation
#              (docs/assets/images/*.svg). Run after changing a diagram:
#                  python tools/docs/diagrams.py
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
# Copyright (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#*****************************************************************************

"""Documentation diagrams of ISOTP-C, built with svgkit (sfs-protocol-library skill).
Preview every diagram (headless Chrome --screenshot) and check its labels against the
code after a change: python tools/docs/diagrams.py"""

from __future__ import annotations

import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from svgkit import (BLUE, GREEN, GREY, LIGHT, LINE, NAVY, ORANGE, RED, TEAL, WHITE,  # noqa: E402
                    Svg)
from hero import isotp_hero  # noqa: E402

OUT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..",
                                    "docs", "assets", "images"))


def frame_types() -> None:
    """The four frame types with their N_PCI bytes and a real example of each."""
    s = Svg(980, 470, "ISO-TP frame types (classical CAN, normal addressing)")
    s.text(20, 30, "Byte", size=13, fill=GREY, anchor="start", weight="bold")
    x0, cw = 250, 88
    for i in range(8):
        s.text(x0 + i * cw + cw / 2, 30, str(i), size=13, fill=GREY, mono=True)
    rows = [
        ("Single Frame (SF)", BLUE, [("0 | SF_DL", 1, NAVY), ("data, 1..7 bytes", 7, BLUE)],
         "02 10 03", "2-byte message 10 03"),
        ("First Frame (FF)", BLUE, [("1 | FF_DL 12 bit", 2, NAVY), ("first 6 data bytes", 6, BLUE)],
         "10 14 00 01 02 03 04 05", "20-byte message"),
        ("FF, 32-bit FF_DL", BLUE, [("10 00", 2, NAVY), ("FF_DL, 32 bit BE", 4, TEAL),
                                    ("2 data", 2, BLUE)],
         "10 00 00 00 10 00 AA BB", "4096-byte message"),
        ("Consecutive Frame (CF)", TEAL, [("2 | SN", 1, NAVY), ("next 1..7 data bytes", 7, TEAL)],
         "21 06 07 08 09 0A 0B 0C", "SN = 1"),
        ("Flow Control (FC)", ORANGE, [("3 | FS", 1, NAVY), ("BS", 1, ORANGE), ("STmin", 1, ORANGE),
                                       ("not used (padding)", 5, LINE)],
         "30 08 14", "CTS, BS 8, STmin 20 ms"),
    ]
    y = 50
    for name, color, fields, example, note in rows:
        s.text(20, y + 30, name, size=14, weight="bold", fill=color, anchor="start")
        x = x0
        for label, cells, fcolor in fields:
            w = cells * cw
            text_color = GREY if fcolor == LINE else WHITE
            s.rect(x + 1, y + 6, w - 2, 38, fill=fcolor if fcolor != LINE else LIGHT,
                   stroke=fcolor, rx=5)
            s.text(x + w / 2, y + 30, label, size=12.5, fill=text_color, weight="bold",
                   mono=label[0].isdigit())
            x += w
        s.text(x0, y + 62, f"e.g. {example}", size=12, fill=GREY, mono=True, anchor="start")
        s.text(x0 + 8 * cw, y + 62, note, size=12, fill=GREY, anchor="end")
        y += 82
    s.text(490, 462, "N_PCI type in the high nibble of byte 0. FF_DL 0 escapes to a 32-bit length "
           "(messages > 4095 bytes).", size=12.5, fill=GREY)
    s.save(OUT + "/frame-types.svg")


def segmentation() -> None:
    """Sequence of one segmented transfer with BS = 2 and STmin, timers marked."""
    s = Svg(900, 560, "Segmented transfer with flow control")
    sx, rx = 200, 700
    for x, title, sub in ((sx, "Sender", "isotp_link_send()"), (rx, "Receiver", "on_message()")):
        s.rect(x - 110, 16, 220, 56, fill=NAVY, stroke=NAVY, rx=10)
        s.text(x, 42, title, size=16, weight="bold", fill=WHITE)
        s.text(x, 62, sub, size=12, fill="#C9D6EA", mono=True)
        s.line(x, 72, x, 540, color=LINE, width=2, dash=True)

    def msg(y, left_to_right, label, color, arrow):
        x1, x2 = (sx, rx) if left_to_right else (rx, sx)
        s.line(x1, y, x2 + (-6 if left_to_right else 6), y, color=color, arrow=arrow, width=2.2)
        s.text((sx + rx) / 2, y - 8, label, size=13, fill=color, mono=True, weight="bold")

    msg(110, True, "FF  10 28 …  (40 bytes, 6 in this frame)", BLUE, "end")
    msg(155, False, "FC  30 02 0A  (CTS, BS 2, STmin 10 ms)", ORANGE, "orange")
    msg(200, True, "CF  21 …", TEAL, "teal")
    msg(250, True, "CF  22 …", TEAL, "teal")
    msg(295, False, "FC  30 02 0A  (block of 2 done)", ORANGE, "orange")
    msg(340, True, "CF  23 …", TEAL, "teal")
    msg(390, True, "CF  24 …", TEAL, "teal")
    msg(435, False, "FC  30 02 0A", ORANGE, "orange")
    msg(480, True, "CF  25 …  (last 6 bytes)", TEAL, "teal")

    def timer(x, y1, y2, label, color, side):
        d = -1 if side == "left" else 1
        s.path(f"M{x + 20 * d},{y1} L{x + 30 * d},{y1} L{x + 30 * d},{y2} L{x + 20 * d},{y2}",
               color=color)
        s.text(x + 38 * d, (y1 + y2) / 2 + 4, label, size=12.5, fill=color,
               anchor="end" if side == "left" else "start", weight="bold")

    timer(sx, 110, 155, "N_Bs", RED, "left")
    timer(sx, 200, 250, "≥ STmin", GREY, "left")
    timer(sx, 250, 295, "N_Bs", RED, "left")
    timer(rx, 155, 200, "N_Cr", RED, "right")
    timer(rx, 435, 480, "N_Cr", RED, "right")
    s.text(rx, 524, "40 bytes complete → on_message()", size=12.5, fill=GREEN, weight="bold")
    s.save(OUT + "/segmentation.svg")


def architecture() -> None:
    s = Svg(960, 560, "ISOTP-C architecture")
    s.rect(140, 20, 680, 90, fill="#FFF7EC", stroke=ORANGE)
    s.text(480, 52, "Application (your firmware or tester)", size=17, weight="bold",
           fill=ORANGE)
    s.text(480, 80, "UDS / OBD / bootloader · on_message() · on_tx_done() · on_rx_error()",
           size=13.5, fill=GREY)
    s.rect(120, 140, 720, 300, fill=WHITE, stroke=BLUE, rx=14, dash=True)
    s.text(140, 166, "ISOTP-C library", size=14, weight="bold", fill=BLUE, anchor="start")
    s.rect(160, 180, 640, 80, fill=LIGHT, stroke=BLUE)
    s.text(480, 210, "isotp_link — segmentation, flow control, timers", size=16, weight="bold")
    s.text(480, 238, "isotp_link_receive() · isotp_link_send() · isotp_link_poll(now_us)",
           size=13, fill=GREY, mono=True)
    for x, title, sub in ((160, "Decoder", "isotp_decode()"),
                          (378, "Encoders", "isotp_encode_*()"),
                          (596, "STmin", "isotp_st_min_*()")):
        s.rect(x, 290, 204, 70, fill=LIGHT, stroke=TEAL)
        s.text(x + 102, 318, title, size=15, weight="bold", fill=TEAL)
        s.text(x + 102, 342, sub, size=12.5, fill=GREY, mono=True)
    s.text(480, 384, "isotp_protocol — codec: SF, FF, CF, FC, padding (pure functions)",
           size=13.5, fill=TEAL)
    s.rect(160, 396, 640, 32, fill=WHITE, stroke=LINE)
    s.text(480, 417, "isotp_defs — frame types, flow status, N_Result codes · isotp_config — sizes",
           size=12.5, fill=GREY)
    s.rect(140, 470, 680, 70, fill=WHITE, stroke=GREY, dash=True)
    s.text(480, 498, "CAN driver and clock (application-owned)", size=15, weight="bold",
           fill=GREY)
    s.text(480, 522, "bxCAN / FDCAN / MCP2515 / SocketCAN / Zephyr CAN · RX interrupt · µs timer",
           size=13, fill=GREY)
    s.line(70, 500, 70, 70, color=TEAL, arrow="teal", width=2.4)
    s.line(70, 500, 138, 500, color=TEAL, width=2.4)
    s.line(70, 70, 138, 70, color=TEAL, arrow="teal", width=2.4)
    s.text(48, 290, "receive: frames → messages", size=13, fill=TEAL, weight="bold", rotate=-90)
    s.line(890, 70, 890, 500, color=ORANGE, arrow="orange", width=2.4)
    s.line(822, 70, 890, 70, color=ORANGE, width=2.4)
    s.line(890, 500, 822, 500, color=ORANGE, arrow="orange", width=2.4)
    s.text(912, 290, "transmit: messages → frames", size=13, fill=ORANGE, weight="bold",
           rotate=90)
    s.save(OUT + "/architecture.svg")


def data_flow() -> None:
    s = Svg(990, 310, "Receive and transmit data flow")
    xs = [20, 214, 408, 602, 796]
    rx = [("CAN RX ISR", "queue frames"), ("main loop / task", "filter the CAN ID"),
          ("isotp_link_receive()", "one frame, now_us"), ("isotp_decode()", "validate N_PCI"),
          ("on_message()", "your code")]
    s.text(20, 28, "Receive", size=14, weight="bold", fill=TEAL, anchor="start")
    for i, ((title, sub), x) in enumerate(zip(rx, xs)):
        s.rect(x, 40, 172, 64, fill=LIGHT, stroke=TEAL if i in (2, 3) else LINE)
        s.text(x + 86, 68, title, size=13.5, weight="bold", mono=title.endswith(")"))
        s.text(x + 86, 90, sub, size=12, fill=GREY)
        if i < 4:
            s.line(x + 172, 72, x + 192, 72, color=TEAL, arrow="teal", width=2)
    tx = [("isotp_link_send()", "SF now, or FF"), ("isotp_link_poll()", "CFs at STmin"),
          ("write callback", "your CAN driver"), ("CAN TX mailbox", "frames out")]
    s.text(990 - 20, 158, "Transmit", size=14, weight="bold", fill=ORANGE, anchor="end")
    s.line(882, 104, 882, 168, color=ORANGE, arrow="orange", width=2)
    for i, ((title, sub), x) in enumerate(zip(tx, reversed(xs[1:]))):
        s.rect(x, 170, 172, 64, fill=LIGHT, stroke=ORANGE if i in (0, 1) else LINE)
        s.text(x + 86, 198, title, size=13.5, weight="bold", mono=title.endswith(")"))
        s.text(x + 86, 220, sub, size=12, fill=GREY)
        if i < 3:
            s.line(x, 202, x - 20, 202, color=ORANGE, arrow="orange", width=2)
    s.text(20, 284, "on_message() runs inside isotp_link_receive(), in the caller's context; "
           "it may answer with isotp_link_send() at once.", size=12.5, fill=GREY,
           anchor="start")
    s.save(OUT + "/data-flow.svg")


def receive_flow() -> None:
    """isotp_link_receive(): what each frame type does (matches src/isotp_link.c)."""
    s = Svg(980, 620, "isotp_link_receive() algorithm")
    cx = 240
    s.rect(cx - 80, 14, 160, 36, fill=NAVY, stroke=NAVY, rx=18)
    s.text(cx, 37, "CAN frame in", size=14, fill=WHITE, weight="bold")
    s.line(cx, 50, cx, 72, arrow="end")
    s.diamond(cx, 116, 320, 84, ["isotp_decode():", "valid SF / FF / CF / FC?"])
    s.line(cx + 160, 116, 520, 116, arrow="end")
    s.text(cx + 170, 108, "no", size=12.5, fill=GREY, anchor="start")
    s.rect(522, 96, 430, 40, fill=WHITE, stroke=GREY, rx=20)
    s.text(737, 121, "invalid_frames++ (ignored)", size=13, fill=GREY, weight="bold")
    s.line(cx, 158, cx, 190, arrow="end")
    s.text(cx + 8, 176, "yes", size=12.5, fill=GREY, anchor="start")
    cases = [
        ("SF", BLUE, ["reception in progress? → on_rx_error(N_UNEXP_PDU)",
                      "on_message(frame data) at once"]),
        ("FF", BLUE, ["reception in progress? → on_rx_error(N_UNEXP_PDU)",
                      "FF_DL > RX buffer → send FC OVFLW, on_rx_error(N_BUFFER_OVFLW)",
                      "else copy, send FC CTS (BS, STmin), start N_Cr"]),
        ("CF", TEAL, ["not receiving → ignore; SN ≠ expected → on_rx_error(N_WRONG_SN)",
                      "too short → invalid_frames++; else copy, restart N_Cr",
                      "last byte → on_message(); BS reached → send FC CTS"]),
        ("FC", ORANGE, ["not waiting for FC → ignore",
                        "CTS → send CFs (BS, STmin) · WAIT → restart N_Bs (≤ N_WFTmax)",
                        "OVFLW / other FS → on_tx_done(N_BUFFER_OVFLW / N_INVALID_FS)"]),
    ]
    y = 192
    for name, color, lines in cases:
        h = 22 + 20 * len(lines)
        s.rect(cx - 60, y, 120, h, fill=color, stroke=color)
        s.text(cx, y + h / 2 + 6, name, size=17, fill=WHITE, weight="bold")
        s.rect(cx + 80, y, 640, h, fill=LIGHT, stroke=color)
        for i, line in enumerate(lines):
            s.text(cx + 96, y + 26 + i * 20, line, size=12.5, anchor="start")
        s.line(cx + 60, y + h / 2, cx + 78, y + h / 2, color=color, width=2)
        y += h + 22
    s.text(490, 606, "Timeouts (N_Bs, N_Cr) and the Consecutive Frames of a transmission are "
           "handled by isotp_link_poll(now_us).", size=12.5, fill=GREY)
    s.save(OUT + "/receive-flow.svg")


def bus_topology() -> None:
    s = Svg(980, 360, "ISO-TP on a CAN bus")
    s.rect(20, 110, 200, 130, fill=NAVY, stroke=NAVY, rx=12)
    s.text(120, 150, "Tester", size=17, weight="bold", fill=WHITE)
    s.text(120, 174, "PC or gateway", size=13, fill="#C9D6EA")
    s.text(120, 196, "ISOTP-C + CAN", size=13, fill="#C9D6EA")
    s.text(120, 218, "sends on 0x7E0 / 0x7DF", size=12, fill="#C9D6EA")
    for y, label in ((150, "CAN_H"), (175, "CAN_L")):
        s.line(220, y, 940, y, color=TEAL if label == "CAN_H" else BLUE, width=3)
        s.text(232, y - 5, label, size=10.5, fill=GREY, anchor="start")
    for x, (name, ids) in zip((340, 540, 740), (("Engine ECU", "7E0 → 7E8"),
                                                 ("Brake ECU", "7E1 → 7E9"),
                                                 ("Body ECU", "7E2 → 7EA"))):
        s.line(x + 60, 175, x + 60, 250, color=TEAL, width=2)
        s.rect(x, 250, 120, 90, fill=LIGHT, stroke=BLUE, rx=10)
        s.text(x + 60, 286, name, size=13, weight="bold")
        s.text(x + 60, 310, ids, size=12, fill=GREY, mono=True)
    s.rect(270, 60, 320, 38, fill=WHITE, stroke=ORANGE, rx=6)
    s.text(430, 84, "Request 7E0: 22 F1 90 (Single Frame)", size=13, fill=ORANGE, weight="bold")
    s.line(300, 120, 560, 120, color=ORANGE, arrow="orange", width=2.4)
    s.rect(610, 60, 340, 38, fill=WHITE, stroke=GREEN, rx=6)
    s.text(780, 84, "Response 7E8: 20 bytes (FF + 2 CF)", size=13, fill=GREEN, weight="bold")
    s.line(900, 120, 620, 120, color=GREEN, width=2.4)
    s.path("M620,120 L630,115 L630,125 z", color=GREEN, fill=GREEN)
    s.text(490, 30, "One link per peer: a pair of CAN IDs. The application routes received "
           "frames to the link by CAN ID.", size=13.5, fill=GREY)
    s.save(OUT + "/bus-topology.svg")


def readme_banner() -> None:
    """README banner: the website hero (text + hero-3d.svg) on its own dark background,
    so it reads the same on GitHub's light and dark themes."""
    w, h = 1280, 520
    font = "Inter, 'Segoe UI', Helvetica, Arial, sans-serif"
    with open(os.path.join(OUT, "hero-3d.svg"), encoding="utf-8") as f:
        hero = f.read()
    size = re.search(r'width="(\d+)" height="(\d+)"', hero)
    box_x, box_y, box_w, box_h = 560, 20, 700, 480
    scale = min(box_w / int(size.group(1)), box_h / int(size.group(2)))
    hw, hh = int(size.group(1)) * scale, int(size.group(2)) * scale
    hero = hero.replace(size.group(0), f'x="{box_x + (box_w - hw) / 2:.0f}" '
                        f'y="{box_y + (box_h - hh) / 2:.0f}" width="{hw:.0f}" height="{hh:.0f}"', 1)
    parts = [
        f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {w} {h}" width="{w}" '
        f'height="{h}" role="img" aria-label="ISOTP-C: ISO 15765-2 (ISO-TP) in portable C">',
        "<title>ISOTP-C: ISO 15765-2 (ISO-TP) in portable C</title>",
        '<defs><radialGradient id="bannerGlow" cx="72%" cy="62%" r="55%">'
        '<stop offset="0" stop-color="#1a6fe0" stop-opacity="0.32"/>'
        '<stop offset="1" stop-color="#1a6fe0" stop-opacity="0"/></radialGradient>'
        '<linearGradient id="bannerBg" x1="0" y1="0" x2="0" y2="1">'
        '<stop offset="0" stop-color="#050b16"/><stop offset="1" stop-color="#071122"/>'
        '</linearGradient></defs>',
        f'<rect width="{w}" height="{h}" rx="18" fill="url(#bannerBg)"/>',
        f'<rect width="{w}" height="{h}" rx="18" fill="url(#bannerGlow)"/>',
        f'<rect x="0.5" y="0.5" width="{w - 1}" height="{h - 1}" rx="18" fill="none" '
        f'stroke="#15294a"/>',
        f'<text x="64" y="118" font-family="{font}" font-size="17" font-weight="700" '
        f'letter-spacing="3" fill="#2f8cff">OPEN-SOURCE EMBEDDED LIBRARY</text>',
    ]
    # The hero headline of extra.landing in mkdocs.yml, in capitals; the last line is the
    # accent. Lower the font size when a line runs into the scene.
    for y, line, color in ((196, "ISO 15765-2", "#e6edf7"), (264, "(ISO-TP)", "#e6edf7"),
                           (332, "IN PORTABLE C", "#2f8cff")):
        parts.append(f'<text x="60" y="{y}" font-family="{font}" font-size="64" '
                     f'font-weight="800" fill="{color}">{line}</text>')
    parts += [
        '<rect x="64" y="368" width="150" height="3" rx="1.5" fill="#2f8cff" opacity="0.5"/>',
        '<circle cx="128" cy="369.5" r="5" fill="#2f8cff"/>',
        f'<text x="64" y="420" font-family="{font}" font-size="19" fill="#9aabc4">'
        "No heap · No RTOS · MISRA C:2012 · 100 % coverage</text>",
        hero,
        "</svg>",
    ]
    path = os.path.join(OUT, "readme-banner.svg")
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(parts) + "\n")
    print("wrote", os.path.relpath(path))


def main() -> None:
    os.makedirs(OUT, exist_ok=True)
    isotp_hero(os.path.join(OUT, "hero-3d.svg"))
    readme_banner()
    frame_types()
    segmentation()
    architecture()
    data_flow()
    receive_flow()
    bus_topology()


if __name__ == "__main__":
    main()
