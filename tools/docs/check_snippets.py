#******************************************************************************
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Compiles every ```c block of docs/*.md against the library.
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

"""Compile the C snippets of the documentation.

    python tools/docs/check_snippets.py [compiler]

Each ```c block of docs/*.md is compiled on its own with
-std=c11 -Wall -Wextra -Wpedantic -Wconversion -Werror. The project data is in
tools/docs/snippets.json (this script is the same in every library):

``stubs``
    A header put before every snippet: the library header and declarations of the
    board functions the snippets stand in for.
``include``
    Include folders (relative to the repository root) for every compilation.
``variants``
    Alternative extra include folders, for example configuration headers; a snippet
    passes when it compiles with any of them. A file path is copied into a temporary
    folder under the name given by ``as``.
``optional``
    Definitions added when a snippet uses a name without defining it:
    ``{"name": ..., "pattern": ..., "code": ...}``.
``fragment_marker``
    Text that marks a snippet which is not a translation unit (for example a
    configuration fragment); such a snippet is compiled alone.
``flags``
    Extra compiler flags (optional).

A snippet that does not compile as a translation unit is tried again as the body of
a function (a few statements). A snippet that repeats a declaration of a public
header passes when its code, without comments, is in one of the headers of the
include folders, so the documentation cannot drift from the headers.
"""

from __future__ import annotations

import glob
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from typing import Any, Dict, List

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
CONFIG = os.path.join(ROOT, "tools", "docs", "snippets.json")


def load_config() -> Dict[str, Any]:
    if not os.path.isfile(CONFIG):
        return {"stubs": None, "include": [".", "inc"], "variants": [[]], "optional": []}
    with open(CONFIG, encoding="utf-8") as f:
        return json.load(f)


def build_sources(block: str, stubs: str, config: Dict[str, Any]) -> List[str]:
    """Stubs, then what the block uses but does not define, then the block: as a
    translation unit and as the body of a function."""
    marker = config.get("fragment_marker")
    if marker and marker in block:
        return [block + "\ntypedef int snippet_translation_unit; /* a fragment */\n"]
    source = stubs
    for item in config.get("optional", []):
        defined = re.search(item["pattern"], block, re.M) is not None
        if not defined and re.search(r"\b" + re.escape(item["name"]) + r"\b", block):
            source += item["code"] + "\n"
    body = "void snippet_body(void);\nvoid snippet_body(void)\n{\n" + block + "\n}\n"
    return [source + block, source + body]


def code_only(text: str) -> str:
    """The code of a C text without comments, with single spaces."""
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    text = re.sub(r"//[^\n]*", " ", text)
    return " ".join(text.split())


def header_code(folders: List[str]) -> List[str]:
    out = []
    for folder in folders:
        for path in sorted(glob.glob(os.path.join(folder, "*.h"))):
            with open(path, encoding="utf-8") as f:
                out.append(code_only(f.read()))
    return out


def snippets():
    for path in sorted(glob.glob(os.path.join(ROOT, "docs", "*.md"))):
        with open(path, encoding="utf-8") as f:
            text = f.read()
        for i, block in enumerate(re.findall(r"```c\n(.*?)```", text, re.S)):
            yield os.path.relpath(path, ROOT), i + 1, block


def variant_dirs(config: Dict[str, Any], tmp: str) -> List[List[str]]:
    out = []
    for n, variant in enumerate(config.get("variants", [[]])):
        dirs = []
        for entry in variant:
            if isinstance(entry, dict):     # a file copied under another name
                folder = os.path.join(tmp, f"variant{n}")
                os.makedirs(folder, exist_ok=True)
                shutil.copyfile(os.path.join(ROOT, entry["file"]), os.path.join(folder, entry["as"]))
                dirs.append(folder)
            else:
                dirs.append(os.path.join(ROOT, entry))
        out.append(dirs)
    return out


def compiles(cc: str, source: str, includes: List[str], tmp: str, flags: List[str]) -> str:
    path = os.path.join(tmp, "snippet.c")
    with open(path, "w", encoding="utf-8") as f:
        f.write(source)
    command = [cc, "-std=c11", "-Wall", "-Wextra", "-Wpedantic", "-Wconversion", "-Werror",
               "-Wno-unused-function", "-Wno-unused-variable", *flags, "-c", path, "-o", os.devnull]
    for folder in includes:
        command += ["-I", folder]
    result = subprocess.run(command, capture_output=True, text=True)
    return "" if result.returncode == 0 else result.stderr


def main() -> int:
    cc = sys.argv[1] if len(sys.argv) > 1 else "gcc"
    config = load_config()
    stubs = ""
    if config.get("stubs"):
        with open(os.path.join(ROOT, config["stubs"]), encoding="utf-8") as f:
            stubs = f.read() + "\n"
    base = [os.path.join(ROOT, d) for d in config.get("include", [".", "inc"])]
    headers = header_code(base)
    flags = config.get("flags", [])
    failures = 0
    count = 0
    with tempfile.TemporaryDirectory() as tmp:
        variants = variant_dirs(config, tmp)
        for page, number, block in snippets():
            count += 1
            if any(code_only(block) in header for header in headers):
                print(f"ok   {page} block {number} (in a header)")
                continue
            errors = [compiles(cc, source, base + extra, tmp, flags)
                      for source in build_sources(block, stubs, config) for extra in variants]
            if all(errors):
                failures += 1
                print(f"FAIL {page} block {number}\n{errors[0]}")
            else:
                print(f"ok   {page} block {number}")
    print(f"{count - failures}/{count} snippets compile")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
