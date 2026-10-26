#******************************************************************************
# SystemFromScratch® documentation toolkit
#
# Description: SVG helpers for the documentation diagrams of SystemFromScratch®
#              protocol libraries: a 2D canvas (Svg) for technical diagrams and an
#              isometric renderer (Iso) for the animated 3D landing-page hero.
#              Import it from tools/docs/diagrams.py.
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
# License:     MIT License
#
# SPDX-License-Identifier: MIT
#*****************************************************************************

"""SVG toolkit for protocol-library documentation.

2D diagrams use presentation attributes only (no CSS), so they render the same in
browsers and in WeasyPrint, on a white rounded card that also works in dark mode.
The 3D hero uses CSS/SMIL animations, which browsers run inside <img> too, and
respects prefers-reduced-motion.

    from svgkit import Svg, hero_scene, NAVY, TEAL
    s = Svg(960, 300, "Packet structure")
    s.rect(20, 50, 200, 46, fill=NAVY, stroke=NAVY)
    s.text(120, 80, "Header", fill="#fff", weight="bold")
    s.save("docs/assets/images/packet-structure.svg")

    hero_scene("docs/assets/images/hero-3d.svg", chip_label="ISOTP-C",
               chip_sub="ISO 15765-2", transceiver="CAN", devices=["ECU 1", "ECU 2", "ECU 3"])
"""

from __future__ import annotations

import math
import os
from typing import List, Sequence, Tuple

# 2D diagram palette
NAVY = "#0B2545"
BLUE = "#1E5AA8"
TEAL = "#0F8B9A"
ORANGE = "#E07A00"
RED = "#C62828"
GREEN = "#2E7D32"
GREY = "#5A6472"
LIGHT = "#F2F5FA"
LINE = "#C9D3E0"
WHITE = "#FFFFFF"

SANS = "Inter, 'Segoe UI', 'DejaVu Sans', Arial, sans-serif"
MONO = "'JetBrains Mono', Consolas, 'DejaVu Sans Mono', monospace"


def _escape(value: str) -> str:
    return value.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")


def _write(path: str, content: str) -> None:
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write(content)
    print("wrote", path)


# ==============================================================================
# 2D DIAGRAMS
# ==============================================================================

class Svg:
    """A white card with boxes, texts, lines, brackets and decision diamonds."""

    def __init__(self, width: int, height: int, title: str) -> None:
        self.parts: List[str] = [
            f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {width} {height}" '
            f'width="{width}" height="{height}" role="img" aria-label="{_escape(title)}">',
            f"<title>{_escape(title)}</title>",
            "<defs>" + "".join(
                f'<marker id="{mid}" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="7" '
                f'markerHeight="7" orient="auto-start-reverse"><path d="M0,0 L10,5 L0,10 z" '
                f'fill="{color}"/></marker>'
                for mid, color in (("arrow", GREY), ("arrow-teal", TEAL),
                                   ("arrow-orange", ORANGE), ("arrow-green", GREEN)))
            + "</defs>",
            f'<rect x="0" y="0" width="{width}" height="{height}" rx="12" fill="{WHITE}"/>',
        ]

    @staticmethod
    def _marker(arrow: str) -> str:
        return {"": "", "end": ' marker-end="url(#arrow)"', "teal": ' marker-end="url(#arrow-teal)"',
                "orange": ' marker-end="url(#arrow-orange)"',
                "green": ' marker-end="url(#arrow-green)"'}[arrow]

    def rect(self, x, y, w, h, fill=LIGHT, stroke=LINE, rx=8, dash=False, sw=1.5) -> None:
        extra = ' stroke-dasharray="6 4"' if dash else ""
        self.parts.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" '
                          f'fill="{fill}" stroke="{stroke}" stroke-width="{sw}"{extra}/>')

    def text(self, x, y, value, size=14, fill=NAVY, weight="normal", anchor="middle",
             mono=False, rotate=0) -> None:
        transform = f' transform="rotate({rotate} {x} {y})"' if rotate else ""
        self.parts.append(f'<text x="{x}" y="{y}" font-family="{MONO if mono else SANS}" '
                          f'font-size="{size}" font-weight="{weight}" fill="{fill}" '
                          f'text-anchor="{anchor}"{transform}>{_escape(value)}</text>')

    def line(self, x1, y1, x2, y2, color=GREY, arrow="", width=1.6, dash=False) -> None:
        extra = ' stroke-dasharray="6 4"' if dash else ""
        self.parts.append(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{color}" '
                          f'stroke-width="{width}"{self._marker(arrow)}{extra}/>')

    def path(self, d, color=GREY, arrow="", width=1.6, fill="none") -> None:
        self.parts.append(f'<path d="{d}" stroke="{color}" stroke-width="{width}" '
                          f'fill="{fill}"{self._marker(arrow)}/>')

    def bracket(self, x1, x2, y, label, color=GREY, size=13) -> None:
        """A bracket under a span of fields, e.g. what the Length or the CRC covers."""
        self.path(f"M{x1},{y - 8} L{x1},{y} L{x2},{y} L{x2},{y - 8}", color=color)
        self.text((x1 + x2) / 2, y + 18, label, size=size, fill=color)

    def diamond(self, cx, cy, w, h, lines: Sequence[str]) -> None:
        """A flowchart decision with one or more text lines."""
        self.parts.append(f'<polygon points="{cx},{cy - h / 2} {cx + w / 2},{cy} {cx},{cy + h / 2} '
                          f'{cx - w / 2},{cy}" fill="#FFF7EC" stroke="{ORANGE}" stroke-width="1.5"/>')
        top = cy - (len(lines) - 1) * 8 + 5
        for i, value in enumerate(lines):
            self.text(cx, top + i * 16, value, size=13)

    def field_row(self, y, fields, height=46) -> None:
        """Frame fields as colored cells: [(name, x, width, [cell texts], color), ...]."""
        for name, x, w, cells, color in fields:
            self.text(x + w / 2, y - 12, name, size=15, weight="bold", fill=color)
            cell = w / len(cells)
            for i, value in enumerate(cells):
                self.rect(x + i * cell, y, cell, height, fill=color, stroke=WHITE, rx=4, sw=2)
                self.text(x + i * cell + cell / 2, y + height / 2 + 5, value, size=14,
                          fill=WHITE, mono=True, weight="bold")

    def save(self, path: str) -> None:
        _write(path, "\n".join(self.parts + ["</svg>"]) + "\n")


# ==============================================================================
# ISOMETRIC 3D
# ==============================================================================

class Iso:
    """Isometric projection (30°) of a scene in world units; collects every projected
    point so the viewBox fits the drawing."""

    def __init__(self, scale: float = 26.0) -> None:
        self.cos30 = math.cos(math.radians(30))
        self.scale = scale
        self.points: List[Tuple[float, float]] = []
        self.parts: List[str] = []

    def p(self, x, y, z=0.0) -> Tuple[float, float]:
        sx = (x - y) * self.cos30 * self.scale
        sy = (x + y) * 0.5 * self.scale - z * self.scale
        self.points.append((sx, sy))
        return sx, sy

    @staticmethod
    def poly(pts, fill, stroke="none", width=1.0) -> str:
        d = " ".join(f"{x:.1f},{y:.1f}" for x, y in pts)
        return (f'<polygon points="{d}" fill="{fill}" stroke="{stroke}" stroke-width="{width}" '
                f'stroke-linejoin="round"/>')

    def box(self, x0, x1, y0, y1, z0, z1, top, left, right, stroke="none", width=1.0) -> None:
        """Visible faces of an axis-aligned box. Draw back to front (small x + y first)."""
        p = self.p
        self.parts += [
            self.poly([p(x0, y1, z0), p(x1, y1, z0), p(x1, y1, z1), p(x0, y1, z1)], left, stroke, width),
            self.poly([p(x1, y0, z0), p(x1, y1, z0), p(x1, y1, z1), p(x1, y0, z1)], right, stroke, width),
            self.poly([p(x0, y0, z1), p(x1, y0, z1), p(x1, y1, z1), p(x0, y1, z1)], top, stroke, width),
        ]

    def path(self, pts, stroke, width, extra="") -> None:
        d = "M" + " L".join(f"{x:.1f},{y:.1f}" for x, y in pts)
        self.parts.append(f'<path d="{d}" fill="none" stroke="{stroke}" stroke-width="{width}" '
                          f'stroke-linecap="round" stroke-linejoin="round"{extra}/>')

    def _matrix(self, face: str) -> str:
        c, s = self.cos30 * self.scale, 0.5 * self.scale
        m = (c, s, -c, s) if face == "top" else (c, s, 0.0, self.scale)
        return f"{m[0]:.3f} {m[1]:.3f} {m[2]:.3f} {m[3]:.3f}"

    def text(self, x, y, z, value, size, color, weight=700, spacing=0.0, face="top") -> None:
        """Text lying on a top face, or standing on a front (y) face."""
        ox, oy = self.p(x, y, z)
        self.parts.append(f'<text transform="matrix({self._matrix(face)} {ox:.1f} {oy:.1f})" '
                          f'font-family="{SANS}" font-size="{size}" font-weight="{weight}" '
                          f'letter-spacing="{spacing}" fill="{color}" text-anchor="middle">'
                          f'{_escape(value)}</text>')

    def disc(self, x, y, z, radius, rings: Sequence[Tuple[float, str]]) -> None:
        """Concentric circles on a top face (servo horn, connector, fan…)."""
        ox, oy = self.p(x, y, z)
        circles = "".join(f'<circle r="{r * radius}" fill="{color}"/>' for r, color in rings)
        self.parts.append(f'<g transform="matrix({self._matrix("top")} {ox:.1f} {oy:.1f})">'
                          f'{circles}</g>')

    def led(self, x, y, z, css="led", color="#3fcf8e", r=2.8) -> None:
        ox, oy = self.p(x, y, z)
        self.parts.append(f'<circle cx="{ox:.1f}" cy="{oy:.1f}" r="{r}" fill="{color}" class="{css}"/>')

    def svg(self, label: str, defs: str = "", style: str = "", pad: int = 24,
            glow_at: Tuple[float, float] = (0, 0)) -> str:
        xs = [x for x, _ in self.points]
        ys = [y for _, y in self.points]
        vx, vy = min(xs) - pad, min(ys) - pad
        vw, vh = max(xs) - min(xs) + 2 * pad, max(ys) - min(ys) + 2 * pad
        gx, gy = self.p(*glow_at)
        return "\n".join([
            f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="{vx:.0f} {vy:.0f} {vw:.0f} {vh:.0f}" '
            f'width="{vw:.0f}" height="{vh:.0f}" role="img" aria-label="{_escape(label)}">',
            f"<title>{_escape(label)}</title>",
            "<defs>" + defs + "</defs>",
            "<style>" + style + "</style>",
            f'<ellipse cx="{gx:.1f}" cy="{gy:.1f}" rx="{vw * 0.55:.0f}" ry="{vh * 0.5:.0f}" '
            f'fill="url(#floorGlow)"/>',
        ] + self.parts + ["</svg>"]) + "\n"


HERO_DEFS = (
    '<linearGradient id="boardTop" x1="0" y1="0" x2="1" y2="1">'
    '<stop offset="0" stop-color="#0f2140"/><stop offset="1" stop-color="#0a1628"/></linearGradient>'
    '<linearGradient id="chipTop" x1="0" y1="0" x2="1" y2="1">'
    '<stop offset="0" stop-color="#1a2a44"/><stop offset="1" stop-color="#0a1322"/></linearGradient>'
    '<radialGradient id="floorGlow" cx="50%" cy="50%" r="50%">'
    '<stop offset="0" stop-color="#1a6fe0" stop-opacity="0.30"/>'
    '<stop offset="0.6" stop-color="#0b2a55" stop-opacity="0.14"/>'
    '<stop offset="1" stop-color="#050b16" stop-opacity="0"/></radialGradient>'
    '<filter id="glow" x="-50%" y="-50%" width="200%" height="200%">'
    '<feGaussianBlur stdDeviation="2.2" result="b"/>'
    '<feMerge><feMergeNode in="b"/><feMergeNode in="SourceGraphic"/></feMerge></filter>'
)

HERO_STYLE = (
    ".flow{stroke-dasharray:4 10;animation:flow 1.6s linear infinite}"
    ".flow-bus{stroke-dasharray:3 16;opacity:.9;animation:flow 2.4s linear infinite}"
    "@keyframes flow{to{stroke-dashoffset:-56}}"
    ".led{animation:pulse 2.4s ease-in-out infinite}"
    ".led-rx{animation:rx 5s linear infinite}"
    "@keyframes pulse{50%{opacity:.35}}"
    "@keyframes rx{0%,30%{opacity:.3}36%,44%{opacity:1}50%,100%{opacity:.3}}"
    ".noise{animation:noise 2.8s ease-in infinite;opacity:0}"
    "@keyframes noise{0%{opacity:0;transform:translate(0,0)}10%{opacity:1}"
    "60%{opacity:.9;transform:translate(-6px,26px) rotate(40deg)}"
    "100%{opacity:0;transform:translate(-10px,46px) rotate(80deg)}}"
    "@media (prefers-reduced-motion:reduce){.flow,.flow-bus,.led,.led-rx,.noise{animation:none}}"
)


def hero_scene(path: str, chip_label: str, chip_sub: str, transceiver: str = "RS-485",
               devices: Sequence[str] = ("ID 1", "ID 2", "ID 3"), target: int = 1,
               device_style: str = "servo") -> None:
    """Animated isometric hero: a board with the library chip, a transceiver and a
    connector; a bus to the devices; red noise falling off the line (rejected by the
    parser); a blue request travelling to devices[target] and a green reply coming back.

    device_style: "servo" (box with a horn disc on top) or "module" (box with a
    connector strip), to match the protocol's typical devices."""
    iso = Iso()
    p = iso.p
    iso.box(0, 10, 0, 7.2, -0.4, 0, "url(#boardTop)", "#0a1628", "#071122", "#1e3a66", 1.2)
    traces = []
    for i in range(6):
        y = 2.3 + i * 0.55
        traces.append([p(2.0, y), p(1.1, y), p(0.7, y - 0.4), p(0.25, y - 0.4)])
        x = 2.3 + i * 0.55
        traces.append([p(x, 2.0), p(x, 1.1), p(x + 0.35, 0.75), p(x + 0.35, 0.25)])
    for i in range(5):
        x = 2.4 + i * 0.6
        traces.append([p(x, 5.4), p(x, 6.2), p(x - 0.3, 6.5), p(x - 0.3, 6.95)])
    for trace in traces:
        iso.path(trace, "#1d5fbf", 1.3, ' opacity="0.7"')
    for trace in ([p(5.4, 3.6), p(6.4, 3.6), p(6.9, 4.1), p(7.4, 4.1)],
                  [p(8.7, 4.1), p(8.9, 4.1), p(8.9, 5.6), p(9.2, 5.9)]):
        iso.path(trace, "#1d5fbf", 1.6)
        iso.path(trace, "#5ea6ff", 1.6, ' class="flow"')
    iso.box(7.4, 8.7, 3.4, 4.8, 0, 0.45, "#14243c", "#0c182b", "#09121f", "#2a4870")
    iso.text(8.05, 4.1, 0.46, transceiver, 0.32, "#6b7d99", 600)
    iso.box(9.2, 10.0, 5.3, 6.5, 0, 0.55, "#c9d6ea", "#8aa0bf", "#6f86a6")
    for i in range(7):
        c = 2.25 + i * 0.45
        iso.box(c - 0.08, c + 0.08, 5.2, 5.45, 0, 0.12, "#8fa6c6", "#6b819f", "#5a6f8b")
        iso.box(5.2, 5.45, c - 0.08, c + 0.08, 0, 0.12, "#8fa6c6", "#6b819f", "#5a6f8b")
    iso.box(2.0, 5.2, 2.0, 5.2, 0, 0.75, "url(#chipTop)", "#0a1322", "#060d18", "#2f8cff", 1.0)
    size = min(0.6, 3.6 / max(1, len(chip_label)))
    iso.text(3.6, 3.55, 0.76, chip_label, size, "#e6edf7", 800, 0.03)
    iso.text(3.6, 4.25, 0.76, chip_sub.upper(), 0.24, "#5ea6ff", 600, 0.06)
    iso.led(2.45, 2.45, 0.76)

    xs = [13.0 + i * 4.4 for i in range(len(devices))]
    for i, (x0, name) in enumerate(zip(xs, devices)):
        iso.box(x0, x0 + 2.4, 3.2, 5.4, -0.4, 1.5, "#1b2a40", "#131f31", "#0e1726", "#2a4870")
        if device_style == "servo":
            iso.disc(x0 + 1.2, 4.3, 1.5, 0.82,
                     [(1.0, "#8aa0bf"), (0.88, "#c9d6ea"), (0.27, "#8aa0bf")])
        else:
            iso.box(x0 + 0.4, x0 + 2.0, 3.6, 4.0, 1.5, 1.65, "#c9d6ea", "#8aa0bf", "#6f86a6")
        iso.text(x0 + 1.2, 5.4, 0.35, name, 0.42, "#9aabc4", 700, face="y")
        iso.led(x0 + 2.0, 5.4, 1.15, "led-rx" if i == target else "led", r=2.6)

    bus_y, bus_z = 6.6, -0.35
    end_x = xs[-1] + 3.2
    bus = [p(10.0, 5.9, -0.2), p(10.6, bus_y, bus_z), p(end_x, bus_y, bus_z)]
    for x0 in xs:
        drop = [p(x0 + 1.2, bus_y, bus_z), p(x0 + 1.2, 5.4, bus_z), p(x0 + 1.2, 5.4, 0.1)]
        iso.path(drop, "#0e2a4f", 5)
        iso.path(drop, "#2f8cff", 1.6, ' opacity="0.8"')
    iso.path(bus, "#0e2a4f", 7)
    iso.path(bus, "#2f8cff", 2.2, ' opacity="0.9"')
    iso.path(bus, "#9cc8ff", 2.2, ' class="flow-bus"')

    for i, (nx, delay) in enumerate(((11.4, 0.0), (12.0, 0.7), (11.7, 1.4), (12.4, 2.1))):
        sx, sy = p(nx, bus_y, bus_z + 0.25)
        iso.parts.append(f'<g transform="translate({sx:.1f} {sy:.1f})"><g class="noise" '
                         f'style="animation-delay:{delay}s"><rect x="-3.5" y="-3.5" width="7" '
                         f'height="7" rx="1.2" fill="#ff5c6c" transform="rotate({20 + 25 * i})"/>'
                         f'</g></g>')

    start = p(10.6, bus_y, bus_z + 0.15)
    stop = p(xs[target] + 1.2, bus_y, bus_z + 0.15)
    track = f"M{start[0]:.1f},{start[1]:.1f} L{stop[0]:.1f},{stop[1]:.1f}"
    for color, glow, motion, visible in (("#2f8cff", "#9cc8ff", "0;1;1;1", "0;1;1;0;0;0"),
                                         ("#3fcf8e", "#a6f0cb", "1;1;1;0", "0;0;0;1;1;0")):
        cubes = "".join(f'<rect x="{-15 + k * 10}" y="-4" width="8" height="8" rx="1.5" '
                        f'fill="{color}" stroke="{glow}" stroke-width="0.8" transform="skewY(30)"/>'
                        for k in range(3))
        iso.parts.append(
            f'<g opacity="0" filter="url(#glow)">{cubes}'
            f'<animateMotion dur="5s" repeatCount="indefinite" path="{track}" '
            f'keyPoints="{motion}" keyTimes="0;0.35;0.55;1" calcMode="linear"/>'
            f'<animate attributeName="opacity" dur="5s" repeatCount="indefinite" '
            f'values="{visible}" keyTimes="0;0.05;0.33;0.4;0.95;1"/></g>')

    label = f"{chip_label} turns a noisy bus into validated packets"
    _write(path, iso.svg(label, HERO_DEFS, HERO_STYLE, glow_at=(12, 5)))
