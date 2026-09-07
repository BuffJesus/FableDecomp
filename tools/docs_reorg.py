#!/usr/bin/env python3
"""Reorganise docs/ from a mapping table and keep links honest.

    python tools/docs_reorg.py --plan                 # show what would move
    python tools/docs_reorg.py --apply                # git mv + rewrite links repo-wide
    python tools/docs_reorg.py --check-links          # every relative .md link resolves
    python tools/docs_reorg.py --check-findings       # every FINDINGS heading has a home
    python tools/docs_reorg.py --check-root           # no scratch tracked at repo root
    python tools/docs_reorg.py --index                # regenerate docs/INDEX.md

Mapping: tools/docs_reorg_map.tsv (source \t destination \t note). `destination` is
relative to docs/; `EXTERNAL:FableForge` / `EXTERNAL:ForgeFSE` moves the file into the
sibling repository's docs/from_fabletlc/ and leaves a pointer in docs/modding/README.md.
"""
from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DOCS = ROOT / "docs"
MAP = ROOT / "tools" / "docs_reorg_map.tsv"
EXTERNAL = {
    "FableForge": Path(r"D:\Code\FableForge\docs\from_fabletlc"),
    "ForgeFSE": Path(r"D:\Code\ForgeFSE-retail-shadow\docs\from_fabletlc"),
}
MODDING_POINTER = DOCS / "modding" / "README.md"
LINK_RE = re.compile(r"(?<!!)\[([^\]]*)\]\(([^)\s]+?\.md)(#[^)]*)?\)")
ROOT_ALLOW = {".gitignore", ".gitattributes", "LICENSE", "CONTRIBUTING.md", "README.md",
              "CLAUDE.md", "AGENTS.md"}
TEXT_EXT = {".md", ".py", ".ps1", ".txt", ".json", ".yml", ".yaml", ".h", ".cpp"}
SKIP_DIRS = {".git", "rebuild/src", "rebuild/tests", "lift/reports", "work", "snapshots",
             "ghidra_out", "debug_build"}


def load_map() -> list[tuple[str, str, str]]:
    rows = []
    for line in MAP.read_text(encoding="utf-8").splitlines():
        if not line.strip() or line.startswith("#") or line.startswith("source\t"):
            continue
        parts = line.split("\t")
        src, dst = parts[0].strip(), parts[1].strip()
        note = parts[2].strip() if len(parts) > 2 else ""
        rows.append((src, dst, note))
    return rows


def git(*args: str) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True,
                          check=True).stdout


def final_location(src: str, dst: str) -> Path | None:
    """Where a docs/<src> file ends up inside this repo, or None if it leaves."""
    if dst.startswith("EXTERNAL:"):
        return None
    return DOCS / dst


def iter_text_files():
    for dirpath, dirnames, filenames in os.walk(ROOT):
        rel = Path(dirpath).relative_to(ROOT).as_posix()
        if any(rel == s or rel.startswith(s + "/") for s in SKIP_DIRS):
            dirnames[:] = []
            continue
        for fn in filenames:
            p = Path(dirpath) / fn
            if p.suffix.lower() in TEXT_EXT:
                yield p


def rewrite_links(moves: dict[Path, Path | None], new_paths: dict[Path, Path]):
    """moves: old abs path -> new abs path (None = external). new_paths: file -> its new path
    for every text file so relative links are computed from the right place."""
    changed = 0
    for f in list(iter_text_files()):
        try:
            text = f.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            continue
        original = text
        here_old = f
        here_new = new_paths.get(f, f)
        if f.suffix.lower() == ".md":
            def sub(m: re.Match) -> str:
                label, target, anchor = m.group(1), m.group(2), m.group(3) or ""
                if "://" in target:
                    return m.group(0)
                abs_target = (here_old.parent / target).resolve()
                if abs_target not in moves:
                    # target did not move, but this file may have: re-relativise
                    if here_new != here_old and abs_target.exists():
                        rel = os.path.relpath(abs_target, here_new.parent).replace("\\", "/")
                        return f"[{label}]({rel}{anchor})"
                    return m.group(0)
                dest = moves[abs_target]
                if dest is None:
                    rel = os.path.relpath(MODDING_POINTER, here_new.parent).replace("\\", "/")
                    return f"[{label}]({rel}#{abs_target.stem.lower()})"
                rel = os.path.relpath(dest, here_new.parent).replace("\\", "/")
                return f"[{label}]({rel}{anchor})"
            text = LINK_RE.sub(sub, text)
        # plain-text `docs/OLD.md` mentions (CLAUDE.md, tools, prose)
        for old, dest in moves.items():
            old_rel = old.relative_to(ROOT).as_posix()
            if old_rel not in text and old_rel.replace("/", "\\") not in text:
                continue
            new_rel = (dest.relative_to(ROOT).as_posix() if dest
                       else f"docs/modding/README.md#{old.stem.lower()}")
            text = text.replace(old_rel, new_rel).replace(old_rel.replace("/", "\\"),
                                                          new_rel.replace("/", "\\"))
        if text != original:
            f.write_text(text, encoding="utf-8")
            changed += 1
    return changed


def apply(plan_only: bool) -> int:
    rows = load_map()
    moves: dict[Path, Path | None] = {}
    externals: list[tuple[Path, str]] = []
    for src, dst, _ in rows:
        s = DOCS / src
        if not s.exists():
            print(f"skip (missing): {src}")
            continue
        if dst.startswith("EXTERNAL:"):
            moves[s.resolve()] = None
            externals.append((s, dst.split(":", 1)[1]))
        else:
            moves[s.resolve()] = (DOCS / dst).resolve()
    for s, d in moves.items():
        print(f"{s.relative_to(ROOT).as_posix()} -> "
              f"{d.relative_to(ROOT).as_posix() if d else 'EXTERNAL'}")
    if plan_only:
        print(f"{len(moves)} moves planned ({len(externals)} external)")
        return 0
    new_paths = {s: d for s, d in moves.items() if d}
    # 1. rewrite links everywhere against the planned layout (files still at old paths)
    changed = rewrite_links(moves, new_paths)
    # 2. move within repo
    for s, d in moves.items():
        if d is None:
            continue
        d.parent.mkdir(parents=True, exist_ok=True)
        git("mv", str(s), str(d))
    # 3. move out to sibling repos + pointer file
    if externals:
        MODDING_POINTER.parent.mkdir(parents=True, exist_ok=True)
        lines = ["# Modding-tooling documentation has moved", "",
                 "These documents describe FableForge / ForgeFSE tooling rather than the decomp "
                 "itself and now live in those repositories (`docs/from_fabletlc/`).", ""]
        for s, repo in sorted(externals, key=lambda t: t[0].name):
            target_dir = EXTERNAL[repo]
            target_dir.mkdir(parents=True, exist_ok=True)
            data = s.read_bytes()
            (target_dir / s.name).write_bytes(data)
            git("rm", "-q", str(s))
            lines.append(f"- <a id=\"{s.stem.lower()}\"></a>`{s.name}` -> {repo} "
                         f"`docs/from_fabletlc/{s.name}`")
        MODDING_POINTER.write_text("\n".join(lines) + "\n", encoding="utf-8")
        git("add", str(MODDING_POINTER))
    print(f"moved {len(moves)} docs ({len(externals)} external); rewrote links in {changed} files")
    return 0


def md_files():
    yield ROOT / "README.md"
    yield ROOT / "CLAUDE.md"
    yield ROOT / "CONTRIBUTING.md"
    yield from DOCS.rglob("*.md")
    yield from (ROOT / "rebuild").glob("*.md")
    yield from (ROOT / "rebuild" / "backlog").glob("*.md")


def check_links() -> int:
    broken = []
    for f in md_files():
        if not f.exists():
            continue
        text = f.read_text(encoding="utf-8", errors="ignore")
        for m in LINK_RE.finditer(text):
            target = m.group(2)
            if "://" in target:
                continue
            if not (f.parent / target).exists():
                broken.append(f"{f.relative_to(ROOT).as_posix()}: {target}")
    for b in broken:
        print("BROKEN", b)
    print(f"links: {len(broken)} broken")
    return 1 if broken else 0


def check_findings() -> int:
    findings = DOCS / "journal" / "FINDINGS_LOG.md"
    if not findings.exists():
        findings = DOCS / "FINDINGS.md"
    if not findings.exists():
        print("findings: no FINDINGS file, nothing to check")
        return 0
    heads = [re.sub(r"[`*]", "", m.group(1)).strip()
             for m in re.finditer(r"^#{2,3}\s+(.+)$", findings.read_text(encoding="utf-8"),
                                  re.M)]
    corpus = ""
    for f in DOCS.rglob("*.md"):
        if "journal" in f.parts or f == findings:
            continue
        corpus += f.read_text(encoding="utf-8", errors="ignore").lower() + "\n"
    orphans = [h for h in heads if h and h.lower() not in corpus]
    for o in orphans:
        print("ORPHAN", o)
    print(f"findings: {len(heads)} headings, {len(orphans)} without a reference-doc home")
    return 1 if orphans and findings.parent.name == "journal" else 0


def check_root() -> int:
    tracked = [l for l in git("ls-files").splitlines() if "/" not in l]
    bad = [t for t in tracked if t not in ROOT_ALLOW]
    for b in bad:
        print("ROOT SCRATCH TRACKED", b)
    print(f"root: {len(bad)} unexpected tracked files")
    return 1 if bad else 0


def build_index() -> int:
    sections = {"": "Entry points", "engine": "Engine reverse engineering",
                "formats": "File formats", "pipeline": "Reconstruction pipeline",
                "scripts": "Quest-script recovery", "modding": "Modding tooling (moved)",
                "journal": "Journal (dated records, not reference)"}
    lines = ["# Documentation index", "",
             "Generated by `tools/docs_reorg.py --index`; do not edit by hand.", ""]
    for key, title in sections.items():
        d = DOCS / key if key else DOCS
        files = sorted(p for p in d.glob("*.md") if p.name != "INDEX.md")
        if key == "journal":
            files = sorted(d.rglob("*.md"))
        if not files:
            continue
        lines += [f"## {title}", ""]
        for p in files:
            first = ""
            for line in p.read_text(encoding="utf-8", errors="ignore").splitlines():
                if line.startswith("#"):
                    first = line.lstrip("# ").strip()
                    break
            rel = p.relative_to(DOCS).as_posix()
            lines.append(f"- [{p.stem}]({rel}) — {first}" if first and first != p.stem
                         else f"- [{p.stem}]({rel})")
        lines.append("")
    (DOCS / "INDEX.md").write_text("\n".join(lines), encoding="utf-8")
    print(f"wrote {DOCS / 'INDEX.md'}")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--plan", action="store_true")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--check-links", action="store_true")
    ap.add_argument("--check-findings", action="store_true")
    ap.add_argument("--check-root", action="store_true")
    ap.add_argument("--index", action="store_true")
    a = ap.parse_args()
    rc = 0
    if a.plan or a.apply:
        rc |= apply(plan_only=not a.apply)
    if a.index:
        rc |= build_index()
    if a.check_links:
        rc |= check_links()
    if a.check_findings:
        rc |= check_findings()
    if a.check_root:
        rc |= check_root()
    if not any(vars(a).values()):
        ap.print_help()
    return rc


if __name__ == "__main__":
    sys.exit(main())
