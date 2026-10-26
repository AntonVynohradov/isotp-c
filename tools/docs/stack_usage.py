#******************************************************************************
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Worst-case stack per public function from GCC call graphs
#              (-fstack-usage -fcallgraph-info=su).
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
#
# License:     MIT License
#
# Copyright (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#*****************************************************************************

"""Worst-case stack depth of the library entry points.

Compile every file of src/ with -fstack-usage -fcallgraph-info=su into one
directory, then:

    python tools/docs/stack_usage.py <dir-with-.ci-files> [--markdown] [--prefix lib_]

Prints, for each public function (prefix: the name of <lib>.cmake), the worst-case stack in bytes and the call
chain that needs it. Calls through function pointers (the write function,
the critical section hooks and the application callbacks) and compiler
builtins (memcpy, memset, memmove) are not included: add the stack of the
application's own functions to the figure of the function that calls them.
"""

from __future__ import annotations

import glob
import os
import re
import sys
from typing import Dict, List, Set, Tuple

NODE = re.compile(r'node: \{ title: "([^"]+)" label: "([^"]*)"')
EDGE = re.compile(r'edge: \{ sourcename: "([^"]+)" targetname: "([^"]+)"')
STACK = re.compile(r"(\d+) bytes \((static|dynamic|bounded)")


def load(directory: str) -> Tuple[Dict[str, int], Dict[str, Set[str]], Dict[str, str]]:
    stack: Dict[str, int] = {}
    kinds: Dict[str, str] = {}
    calls: Dict[str, Set[str]] = {}
    for path in glob.glob(os.path.join(directory, "*.ci")):
        with open(path, encoding="utf-8") as f:
            text = f.read()
        for title, label in NODE.findall(text):
            m = STACK.search(label.replace("\\n", "\n"))
            if m:
                stack[title] = int(m.group(1))
                kinds[title] = m.group(2)
        for src, dst in EDGE.findall(text):
            calls.setdefault(src, set()).add(dst)
    return stack, calls, kinds


def worst(name: str, stack: Dict[str, int], calls: Dict[str, Set[str]],
          path: List[str]) -> Tuple[int, List[str]]:
    if name in path:
        raise RuntimeError("recursion: " + " -> ".join(path + [name]))
    own = stack.get(name, 0)
    best, best_chain = 0, []
    for callee in sorted(calls.get(name, ())):
        if callee == "__indirect_call" or callee not in stack:
            continue  # callbacks and builtins are not part of the library figure
        depth, chain = worst(callee, stack, calls, path + [name])
        if depth > best:
            best, best_chain = depth, chain
    return own + best, [name] + best_chain


def short(name: str) -> str:
    return name.split(":")[-1].split(".")[0]


def default_prefix() -> str:
    """Symbol prefix of the library: the name of the <lib>.cmake file at the repository root."""
    root = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
    names = sorted(f[:-6] for f in os.listdir(root) if f.endswith(".cmake"))
    return names[0] + "_" if names else ""


def main() -> int:
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    stack, calls, kinds = load(sys.argv[1])
    markdown = "--markdown" in sys.argv[2:]
    prefix = sys.argv[sys.argv.index("--prefix") + 1] if "--prefix" in sys.argv else default_prefix()
    public = sorted(n for n in stack if n.startswith(prefix))
    dynamic = [n for n, k in kinds.items() if k != "static"]
    if dynamic:
        print("warning: not static:", ", ".join(dynamic))
    if markdown:
        print("| Function | Worst case (bytes) | Chain |")
        print("| --- | ---: | --- |")
    for name in public:
        depth, chain = worst(name, stack, calls, [])
        text = " -> ".join(short(c) for c in chain)
        if markdown:
            print(f"| `{name}()` | {depth} | {text} |")
        else:
            print(f"{depth:5d}  {name:28s} {text}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
