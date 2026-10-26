#******************************************************************************
# ISOTP-C: ISO 15765-2 (ISO-TP) Implementation
#
# Project:     ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
# Description: Builds the printable PDF user manual from the MkDocs sources
#              (docs/*.md, chapter order from the mkdocs.yml navigation,
#              settings from mkdocs.yml -> extra.manual).
#
# Author:      Anton Vynohradov
# Email:       avynohradov@systemfromscratch.com
#
# License:     MIT License
# Copyright (c) 2026 Anton Vynohradov
#
# SPDX-License-Identifier: MIT
#*****************************************************************************

"""Build the user manual of a SystemFromScratch® library as a print-ready A4 PDF.

Usage (from the repository root):

    python tools/docs/build_manual.py [--output FILE] [--html FILE]

The manual has a cover, an imprint, a table of contents with page numbers,
numbered chapters, appendices, running headers, page numbers and PDF bookmarks.
Everything project-specific comes from mkdocs.yml -> extra.manual:

    title, subtitle, eyebrow, version_header, version_macro, output, cover_image,
    appendices, unnumbered, web_only, keywords, vendor_trademarks

It is rendered with WeasyPrint, which needs the Pango library.
"""

from __future__ import annotations

import argparse
import datetime
import html
import os
import re
import sys
from typing import Dict, List, Optional, Tuple

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
DOCS = os.path.join(ROOT, "docs")
HERE = os.path.dirname(os.path.abspath(__file__))

AUTHOR = "Anton Vynohradov"
COMPANY = "SystemFromScratch&reg;"
EMAIL = "avynohradov@systemfromscratch.com"
ORCID = "0009-0005-6965-0911"
BRAND_LOGO = "assets/images/brand/systemfromscratch-logo-on-dark.png"

# On Windows, WeasyPrint loads Pango from MSYS2 if the user did not say otherwise.
if os.name == "nt" and "WEASYPRINT_DLL_DIRECTORIES" not in os.environ:
    for candidate in (r"C:\msys64\mingw64\bin", r"C:\msys64\ucrt64\bin"):
        if os.path.isfile(os.path.join(candidate, "libpango-1.0-0.dll")):
            os.environ["WEASYPRINT_DLL_DIRECTORIES"] = candidate
            break


# ==============================================================================
# CONFIGURATION
# ==============================================================================

def load_mkdocs() -> dict:
    """mkdocs.yml, tolerating its !!python/name tags."""
    import yaml

    class Loader(yaml.SafeLoader):
        pass

    Loader.add_multi_constructor("tag:yaml.org,2002:python/", lambda loader, suffix, node: None)
    with open(os.path.join(ROOT, "mkdocs.yml"), encoding="utf-8") as f:
        return yaml.load(f, Loader=Loader)


def nav_pages(nav) -> List[str]:
    """Flatten the mkdocs navigation into the list of page paths, in order."""
    pages: List[str] = []
    for item in nav:
        if isinstance(item, str):
            pages.append(item)
        elif isinstance(item, dict):
            for value in item.values():
                if isinstance(value, str):
                    pages.append(value)
                else:
                    pages.extend(nav_pages(value))
    return pages


def library_version(manual: dict) -> str:
    header = manual.get("version_header")
    macro = manual.get("version_macro")
    if header and macro:
        with open(os.path.join(ROOT, header), encoding="utf-8") as f:
            match = re.search(r"#define\s+" + re.escape(macro) + r'\s+"([^"]+)"', f.read())
        if match:
            return match.group(1)
    return "0.0.0"


# ==============================================================================
# MARKDOWN
# ==============================================================================

def markdown_converter():
    import markdown

    extensions = [
        "abbr", "admonition", "attr_list", "def_list", "footnotes", "md_in_html", "tables",
        "toc", "pymdownx.details", "pymdownx.highlight", "pymdownx.inlinehilite",
        "pymdownx.snippets", "pymdownx.superfences", "pymdownx.tasklist",
    ]
    config = {
        "toc": {"permalink": False},
        "pymdownx.highlight": {"use_pygments": True},
        "pymdownx.snippets": {"base_path": [ROOT], "check_paths": True},
    }
    try:
        from material.extensions import emoji as material_emoji

        extensions.append("pymdownx.emoji")
        config["pymdownx.emoji"] = {"emoji_index": material_emoji.twemoji,
                                    "emoji_generator": material_emoji.to_svg}
    except ImportError:
        pass
    return markdown.Markdown(extensions=extensions, extension_configs=config)


def strip_front_matter(text: str) -> str:
    if text.startswith("---\n"):
        end = text.find("\n---\n", 4)
        if end > 0:
            return text[end + 5:]
    return text


def strip_tags(value: str) -> str:
    return html.unescape(re.sub(r"<[^>]+>", "", value)).strip()


def slug_of(page: str) -> str:
    return re.sub(r"[^a-z0-9]+", "-", page[:-3].lower()).strip("-")


# ==============================================================================
# CHAPTERS
# ==============================================================================

class Chapter:
    def __init__(self, page: str, label: Optional[str]) -> None:
        self.page = page
        self.slug = slug_of(page)
        self.label = label  # "3", "A" or None (unnumbered)
        self.title = ""
        self.body = ""
        self.toc: List[Tuple[int, str, str, str]] = []  # level, id, number, title


def rewrite_links(body: str, chapter: Chapter, slugs: Dict[str, str], site_url: str) -> str:
    """Prefix ids per chapter, turn page links into in-document references,
    and print the address of external links for paper readers."""
    body = re.sub(r'id="([^"]+)"', lambda m: f'id="{chapter.slug}--{m.group(1)}"', body)

    def href(match: re.Match) -> str:
        target = html.unescape(match.group(1))
        if re.match(r"^(https?:|mailto:)", target):
            return match.group(0)
        if target.startswith("#"):
            return f'class="xref" href="#{chapter.slug}--{target[1:]}"'
        path, _, fragment = target.partition("#")
        path = os.path.normpath(path).replace("\\", "/")
        if path in slugs:
            anchor = f"{slugs[path]}--{fragment}" if fragment else slugs[path]
            return f'class="xref" href="#{anchor}"'
        if path.endswith(".md"):
            url = site_url.rstrip("/") + "/" + ("" if path == "index.md" else path[:-3] + "/")
            return f'href="{url}{"#" + fragment if fragment else ""}"'
        return match.group(0)

    body = re.sub(r'href="([^"]+)"', href, body)

    def show_url(match: re.Match) -> str:
        url, text = match.group(1), match.group(2)
        shown = re.sub(r"^(mailto:|https?://)(www\.)?", "", url)
        if shown.rstrip("/") in strip_tags(text):
            return match.group(0)
        return f'{match.group(0)}<span class="link-url"> ({html.escape(shown)})</span>'

    return re.sub(r'<a href="((?:https?:|mailto:)[^"]+)">(.*?)</a>', show_url, body)


def number_headings(chapter: Chapter) -> None:
    section = 0

    def heading(match: re.Match) -> str:
        nonlocal section
        level, attrs, inner = int(match.group(1)), match.group(2), match.group(3)
        inner = re.sub(r'<span class="link-url">.*?</span>', "", inner)
        id_match = re.search(r'id="([^"]+)"', attrs)
        anchor = id_match.group(1) if id_match else ""
        title = strip_tags(inner)
        if level == 1:
            chapter.title = title
            number = chapter.label or ""
            chapter.toc.append((1, chapter.slug, number, title))
            kind = "Appendix" if number.isalpha() else "Chapter"
            eyebrow = f'<span class="eyebrow">{kind} {number}</span>' if number else ""
            return f'<h1{attrs}>{eyebrow}<span class="title">{inner}</span></h1>'
        section += 1
        number = f"{chapter.label}.{section}" if chapter.label else ""
        chapter.toc.append((2, anchor, number, title))
        prefix = f'<span class="num">{number}</span>' if number else ""
        return f"<h2{attrs}>{prefix}{inner}</h2>"

    chapter.body = re.sub(r"<h([12])([^>]*)>(.*?)</h\1>", heading, chapter.body, flags=re.S)


def build_chapters(pages: List[str], manual: dict, site_url: str) -> List[Chapter]:
    appendices = list(manual.get("appendices", ["changelog.md", "LICENSE.md"]))
    unnumbered = list(manual.get("unnumbered", ["about.md"]))
    numbered = [p for p in pages if p not in appendices and p not in unnumbered]
    ordered: List[Tuple[str, Optional[str]]] = [(p, str(i + 1)) for i, p in enumerate(numbered)]
    ordered += [(p, None) for p in pages if p in unnumbered]
    ordered += [(p, chr(ord("A") + i)) for i, p in enumerate(a for a in appendices if a in pages)]
    slugs = {page: slug_of(page) for page, _ in ordered}

    md = markdown_converter()
    chapters: List[Chapter] = []
    for page, label in ordered:
        with open(os.path.join(DOCS, page), encoding="utf-8") as f:
            text = strip_front_matter(f.read())
        md.reset()
        chapter = Chapter(page, label)
        chapter.body = rewrite_links(md.convert(text), chapter, slugs, site_url)
        number_headings(chapter)
        chapters.append(chapter)
    return chapters


# ==============================================================================
# DOCUMENT
# ==============================================================================

def toc_html(chapters: List[Chapter]) -> str:
    rows = []
    for chapter in chapters:
        for level, anchor, number, title in chapter.toc:
            css = "toc-chapter" if level == 1 else "toc-section"
            wide = ""
            if level == 1 and chapter.label and chapter.label.isalpha():
                number = f"Appendix {number}"
                wide = " wide"
            num = f'<span class="num{wide}">{html.escape(number)}</span>' if number else ""
            rows.append(f'<li class="{css}"><a href="#{anchor}">{num}'
                        f'<span class="title">{html.escape(title)}</span></a></li>')
    return ('<section class="toc" id="contents"><h1 class="front">Contents</h1><ul>'
            + "".join(rows) + "</ul></section>")


def front_matter(manual: dict, version: str, date: str, site_url: str, repo_url: str,
                 license_appendix: str) -> str:
    title = html.escape(manual.get("title", "Library"))
    subtitle = html.escape(manual.get("subtitle", ""))
    cover = manual.get("cover_image", "")
    cover_img = f'<img class="cover-art" src="{cover}" alt="">' if cover else ""
    eyebrow = html.escape(manual.get("eyebrow", ""))
    eyebrow_div = f'<div class="cover-eyebrow">{eyebrow}</div>' if eyebrow else ""
    vendor = manual.get("vendor_trademarks", "")
    vendor_p = (f"<p>{html.escape(vendor)} All other trademarks are the property of their "
                f"respective owners.</p>") if vendor else ""
    where = f" and in Appendix {license_appendix}" if license_appendix else ""
    year = datetime.date.today().year
    return f"""
<section class="cover">
  <div class="cover-top">
    <img class="cover-logo" src="{BRAND_LOGO}" alt="{COMPANY}">
  </div>
  {eyebrow_div}
  <div class="cover-title">{title}</div>
  <div class="cover-subtitle">{subtitle}</div>
  <div class="cover-rule"></div>
  <div class="cover-kind">User Manual</div>
  {cover_img}
  <div class="cover-meta">
    <div>Version {version} &middot; {date}</div>
    <div>{AUTHOR}</div>
    <div>systemfromscratch.com</div>
  </div>
</section>

<section class="imprint">
  <h1 class="front">Imprint</h1>
  <div class="imprint-brand"><img src="{BRAND_LOGO}" alt="{COMPANY}"></div>
  <p><strong>{title} User Manual</strong><br>
  Library version {version}<br>
  Edition: {date}</p>

  <p><strong>Author</strong><br>
  {AUTHOR}<br>
  {COMPANY} &mdash; systemfromscratch.com<br>
  GitHub: github.com/AntonVynohradov<br>
  LinkedIn: linkedin.com/in/vynohradov<br>
  ORCID: orcid.org/{ORCID}<br>
  E-mail: {EMAIL}</p>

  <p><strong>Online</strong><br>
  Documentation: {html.escape(site_url)}<br>
  Source code: {html.escape(repo_url)}</p>

  <p><strong>Copyright &copy; {year} {AUTHOR}.</strong></p>

  <p>The {title} library and this manual are licensed under the Apache License,
  Version 2.0 (the &ldquo;License&rdquo;); you may not use these files except in
  compliance with the License. You may obtain a copy of the License at
  http://www.apache.org/licenses/LICENSE-2.0{where}. Unless required by applicable law or
  agreed to in writing, software distributed under the License is distributed on an
  &ldquo;AS IS&rdquo; BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
  or implied. See the License for the specific language governing permissions and
  limitations under the License.</p>

  <p>{COMPANY} and the {COMPANY} logo are registered trademarks. The Apache License 2.0
  grants no rights to use them; they may not be used to endorse or promote products
  derived from {title} without prior written permission.</p>

  {vendor_p}

  <p>This manual is generated from the same sources as the online documentation. The
  information in it was checked against the library version stated above; later
  versions may differ.</p>
</section>
"""


def build_html(config: dict, manual: dict, version: str) -> str:
    site_url = config.get("site_url", "")
    repo_url = config.get("repo_url", "")
    web_only = tuple(manual.get("web_only", ["index.md", "reference/"]))
    pages = [p for p in nav_pages(config["nav"]) if not p.startswith(web_only)]
    chapters = build_chapters(pages, manual, site_url)
    date = datetime.date.today().strftime("%B %Y")
    license_appendix = next((c.label for c in chapters
                             if c.page.lower() == "license.md" and c.label), "")

    from pygments.formatters import HtmlFormatter

    title = manual.get("title", "Library")
    with open(os.path.join(HERE, "manual.css"), encoding="utf-8") as f:
        css = f.read()
    css += HtmlFormatter(style="friendly").get_style_defs(".highlight")
    css = css.replace("{{VERSION}}", version).replace("{{TITLE}}", title)

    body = [front_matter(manual, version, date, site_url, repo_url, license_appendix),
            toc_html(chapters)]
    for chapter in chapters:
        kind = "appendix" if chapter.label and chapter.label.isalpha() else "chapter"
        body.append(f'<section class="{kind}" id="{chapter.slug}">{chapter.body}</section>')

    description = html.escape(" ".join(str(config.get("site_description", "")).split()))
    keywords = html.escape(manual.get("keywords", ""))
    return f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>{html.escape(title)} User Manual {version}</title>
<meta name="author" content="{AUTHOR}">
<link rel="author" href="https://orcid.org/{ORCID}">
<meta name="description" content="{description}">
<meta name="keywords" content="{keywords}">
<meta name="generator" content="tools/docs/build_manual.py">
<style>{css}</style>
</head>
<body>
{"".join(body)}
</body>
</html>
"""


# ==============================================================================
# MAIN
# ==============================================================================

def main(argv: Optional[List[str]] = None) -> int:
    config = load_mkdocs()
    manual = (config.get("extra") or {}).get("manual") or {}
    version = library_version(manual)
    pattern = manual.get("output", "build/manual/User_Manual_v{version}.pdf")
    default_output = os.path.join(ROOT, pattern.format(version=version))

    parser = argparse.ArgumentParser(description="Build the PDF user manual.")
    parser.add_argument("--output", default=default_output, help="PDF file to write")
    parser.add_argument("--html", help="also write the intermediate HTML (for debugging)")
    args = parser.parse_args(argv)

    document = build_html(config, manual, version)
    if args.html:
        with open(args.html, "w", encoding="utf-8") as f:
            f.write(document)

    try:
        import weasyprint
    except (ImportError, OSError) as error:
        print(f"WeasyPrint is not available: {error}\n"
              "Install it with 'python -m pip install weasyprint' and install Pango "
              "(Linux: libpango-1.0-0 libpangoft2-1.0-0; Windows: MSYS2 "
              "mingw-w64-x86_64-pango).", file=sys.stderr)
        return 1

    os.makedirs(os.path.dirname(os.path.abspath(args.output)), exist_ok=True)
    weasyprint.HTML(string=document, base_url=DOCS + os.sep).write_pdf(args.output)
    print(f"wrote {os.path.relpath(args.output, ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
