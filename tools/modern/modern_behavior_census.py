#!/usr/bin/env python3
"""Behaviour census: do the reconstructed functions still behave the same under the fork's compilers?

Every behaviour test that #includes the reconstructed source it checks (rebuild/tests/**/*_test.cpp
containing "src/compiled") is compiled, linked and run three ways:

  vc71       the parity compiler (the baseline: a test failing here is not a fork regression)
  modern86   VS 2022, 32-bit, /std:c++17 /permissive /Zc:wchar_t- /Zc:forScope- (VC7.1 for-loop
             scope), against the census's header copy with the PDB size asserts off (fork step 1)
  modern64   VS 2022, 64-bit, against the census's patched header copy (pointer-size check and
             PDB size asserts off), i.e. fork step 2 before any 64-bit type work

A run passes when the test exits 0. Outcomes: pass, fail (non-zero exit), crash (exception exit
code), timeout, compile_fail, link_fail. Nothing in the parity tree is written; objects and
executables go to work/modern_behavior/<config>/, the report to docs/modern/MODERN_BEHAVIOR_CENSUS.md
(+ .tsv of every test whose outcome differs from vc71).

    python tools/modern/modern_behavior_census.py [--jobs N] [--limit N]
"""
from __future__ import annotations

import argparse
import collections
import concurrent.futures as cf
import json
import os
import re
import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
WORK = ROOT / "work" / "modern_behavior"
VC71 = Path(r"D:\Tools\vc71")
OUT_MD = ROOT / "docs" / "modern" / "MODERN_BEHAVIOR_CENSUS.md"
OUT_TSV = ROOT / "docs" / "modern" / "MODERN_BEHAVIOR_CENSUS.tsv"
CONFIGS = ("vc71", "modern86", "modern64")


# 32-bit shortcuts in a test's harness (the test file and the non-engine files it includes):
# x64 failures with one of these say more about the harness than about the reconstructed code.
HARNESS_IDIOMS = {
    "fixed 32-bit storage array": re.compile(r"unsigned(?: long| int)?\s+\w+\[\d+\]"),
    "hard-coded byte offset": re.compile(r"\(\s*(?:unsigned )?char\s*\*\s*\)\s*[\w>-]+\s*\+\s*(?:0x)?[0-9a-fA-F]+\b"),
    "cast of base + constant offset": re.compile(r"reinterpret_cast<[^>]+\*>\(\s*[\w.\[\]>-]+\s*\+\s*(?:0x)?[0-9a-fA-F]+\s*\)"),
    "sizeof/4 storage": re.compile(r"sizeof\([^)]*\)\s*/\s*4"),
}
INCLUDE_RE = re.compile(r'#include\s+"([^"]+)"')


def harness_idioms(test: Path) -> list[str]:
    seen, texts, stack = set(), [], [test]
    while stack:
        p = stack.pop().resolve()
        if p in seen or not p.exists():
            continue
        seen.add(p)
        if "src" in p.parts and "compiled" in p.parts:   # reconstructed code is not harness
            continue
        s = p.read_text(encoding="utf-8", errors="replace")
        texts.append(s)
        for m in INCLUDE_RE.findall(s):
            for base in (p.parent, ROOT / "rebuild" / "include"):
                if (base / m).exists():
                    stack.append(base / m)
                    break
    text = "\n".join(texts)
    return [name for name, rx in HARNESS_IDIOMS.items() if rx.search(text)]


def tests() -> list[Path]:
    out = []
    for p in sorted((ROOT / "rebuild" / "tests").rglob("*_test.cpp")):
        if "src/compiled" in p.read_text(encoding="utf-8", errors="replace"):
            out.append(p)
    return out


def retail_bitset_tree() -> Path:
    """The census header copy with CThing::UsingComponents as fable::compat::retail_bitset<274>
    (VC7.1's 36-byte storage) instead of the current library's 40-byte std::bitset."""
    src = ROOT / "work" / "modern_census" / "include_x64"
    dst = WORK / "include_retail_bitset"
    if dst.exists():
        shutil.rmtree(dst)
    shutil.copytree(src, dst)
    world = dst / "fable_world.h"
    t = world.read_text(encoding="utf-8", errors="surrogateescape")
    assert t.count("std::bitset<274> UsingComponents;") == 1
    t = t.replace("std::bitset<274> UsingComponents;", "fable::compat::retail_bitset<274> UsingComponents;")
    t = '#include "' + (ROOT / "rebuild/modern/compat/include/fable/compat/retail_bitset.hpp").as_posix() + '"\n' + t
    world.write_text(t, encoding="utf-8", errors="surrogateescape")
    for name in ("src", "tests"):   # same sibling junctions as the census copy
        link = WORK / name
        if not link.exists():
            subprocess.run(["cmd", "/c", "mklink", "/J", str(link), str(ROOT / "rebuild" / name)], capture_output=True)
    return dst


def link_trees() -> None:
    """Headers in the census copy include "../../src/..." relative to themselves; junction the copy's
    siblings to the real rebuild/src and rebuild/tests so those resolve."""
    for name in ("src", "tests"):
        link = ROOT / "work" / "modern_census" / name
        if not link.exists():
            subprocess.run(["cmd", "/c", "mklink", "/J", str(link), str(ROOT / "rebuild" / name)], capture_output=True)


def toolchain(config: str, include_override: Path | None = None) -> tuple[list[str], dict]:
    if config == "vc71":
        env = dict(os.environ)
        env["PATH"] = str(VC71 / "bin") + ";" + env["PATH"]
        env["INCLUDE"] = f"{VC71 / 'include'};{ROOT / 'rebuild' / 'include'}"
        env["LIB"] = str(VC71 / "lib")
        return [str(VC71 / "bin" / "cl.exe"), "/nologo", "/O2", "/MT", "/W0"], env
    arch = "x86" if config == "modern86" else "x64"
    env = json.load(open(ROOT / "work" / "modern_census" / f"env_{arch}.json"))
    # Both modern configs use the census copy with the PDB size asserts off: the asserts are known
    # layout differences (MODERN_COMPILE_CENSUS), and this census measures behaviour.
    include = include_override or ROOT / "work" / "modern_census" / "include_x64"
    # The real rebuild/include stays on the path after the copy: tests reach siblings through
    # "../tests/..." and "../src/..." includes that resolve relative to it.
    env["INCLUDE"] = env["INCLUDE"] + ";" + str(include) + ";" + str(ROOT / "rebuild" / "include")
    cl = shutil.which("cl", path=env["PATH"])
    return [cl, "/nologo", "/O2", "/MT", "/W0", "/std:c++17", "/permissive", "/Zc:wchar_t-", "/Zc:forScope-", "/GS-", "/EHsc"], env   # /GS-: VC7.1 had no stack cookies


def run_one(test: Path, config: str, cl: list[str], env: dict) -> tuple[str, str]:
    rel = test.relative_to(ROOT / "rebuild" / "tests")
    # A neutral exe name: Windows' installer detection demands elevation for names containing
    # "install", "setup", "update", ... (WinError 740 on the file-installer tests).
    out = WORK / config / rel.parent / test.stem
    out.mkdir(parents=True, exist_ok=True)
    exe = out / "run.exe"
    # Like the landing pipeline, link the test with the source it checks
    # (rebuild/src/compiled/<shard>/<name>.cpp) unless the test already includes it.
    source = ROOT / "rebuild" / "src" / "compiled" / rel.parent / (test.stem[: -len("_test")] + ".cpp")
    inputs = [str(test)]
    if source.exists() and source.name not in test.read_text(encoding="utf-8", errors="replace"):
        inputs.append(str(source))
    r = subprocess.run(cl + [f"/Fe{exe}", f"/Fo{out}\\", *inputs], env=env, capture_output=True, text=True,
                       cwd=str(test.parent))
    if r.returncode or not exe.exists():
        text = r.stdout + r.stderr
        kind = "link_fail" if "LNK" in text and " error C" not in text else "compile_fail"
        first = next((l.strip() for l in text.splitlines() if " error " in l), "")
        return kind, first[:200]
    try:
        p = subprocess.run([str(exe)], capture_output=True, text=True, timeout=30, cwd=str(out))
    except subprocess.TimeoutExpired:
        return "timeout", ""
    except OSError as e:
        return "crash", f"could not start: {e}"[:200]
    if p.returncode == 0:
        return "pass", ""
    code = p.returncode & 0xFFFFFFFF
    if code >= 0xC0000000:
        return "crash", f"exception 0x{code:08X}"
    return "fail", (p.stdout.strip().splitlines() or [""])[-1][:200]


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 4) - 2))
    ap.add_argument("--limit", type=int)
    ap.add_argument("--retail-bitset", action="store_true",
                    help="modern configs use fable::compat::retail_bitset for CThing::UsingComponents")
    args = ap.parse_args()
    all_tests = tests()[: args.limit] if args.limit else tests()
    link_trees()
    override = retail_bitset_tree() if args.retail_bitset else None
    chains = {c: toolchain(c, override if c != "vc71" else None) for c in CONFIGS}
    results: dict[str, dict[str, tuple[str, str]]] = collections.defaultdict(dict)
    with cf.ThreadPoolExecutor(max_workers=args.jobs) as ex:
        futs = {ex.submit(run_one, t, c, *chains[c]): (t, c) for t in all_tests for c in CONFIGS}
        for f in cf.as_completed(futs):
            t, c = futs[f]
            results[str(t.relative_to(ROOT / "rebuild" / "tests")).replace("\\", "/")][c] = f.result()

    counts = {c: collections.Counter(r[c][0] for r in results.values()) for c in CONFIGS}
    baseline_pass = [t for t, r in results.items() if r["vc71"][0] == "pass"]
    kept = {c: sum(1 for t in baseline_pass if results[t][c][0] == "pass") for c in CONFIGS[1:]}
    regressions = {c: [t for t in baseline_pass if results[t][c][0] != "pass"] for c in CONFIGS[1:]}

    lines = [
        "# Modern behaviour census (fork steps 1-2, behaviour)",
        "",
        "Generated by `tools/modern/modern_behavior_census.py`; do not hand-edit. Each behaviour test that",
        "`#include`s the reconstructed source it checks is built and run with the parity compiler (VC7.1),",
        "VS 2022 x86, and VS 2022 x64 (census header copy: pointer-size check and PDB size asserts off).",
        "A pass is exit code 0. [MODERN_BEHAVIOR_CENSUS.tsv](MODERN_BEHAVIOR_CENSUS.tsv) lists every test whose",
        "outcome differs from VC7.1.",
        "",
        "Modern configs' `CThing::UsingComponents`: " + (
            "`fable::compat::retail_bitset<274>` (VC7.1's 36-byte layout; `--retail-bitset`)." if args.retail_bitset
            else "the current library's `std::bitset<274>` (40 bytes)."),
        "",
        f"Tests: **{len(results)}**",
        "",
        "| Outcome | VC7.1 | VS 2022 x86 | VS 2022 x64 |",
        "|---|---:|---:|---:|",
    ]
    for k in ("pass", "fail", "crash", "timeout", "compile_fail", "link_fail"):
        lines.append(f"| {k} | " + " | ".join(str(counts[c].get(k, 0)) for c in CONFIGS) + " |")
    lines += ["",
              f"Of the {len(baseline_pass)} tests that pass under VC7.1: **{kept['modern86']}** still pass under VS 2022 x86 "
              f"and **{kept['modern64']}** under VS 2022 x64.", ""]
    runtime64 = [t for t in regressions["modern64"] if results[t]["modern64"][0] in ("fail", "crash", "timeout")]
    tagged = {t: harness_idioms(ROOT / "rebuild" / "tests" / t) for t in runtime64}
    idiom_counts = collections.Counter(k for v in tagged.values() for k in v)
    lines += [f"x64 runtime regressions: **{len(runtime64)}**; **{sum(1 for v in tagged.values() if v)}** have a 32-bit "
              "shortcut in their test harness (" + ", ".join(f"{n} {k}" for k, n in idiom_counts.most_common()) +
              "), i.e. the test fixture, not the reconstructed code, assumes 32-bit layouts.", ""]
    for c, title in (("modern86", "x86"), ("modern64", "x64")):
        kinds = collections.Counter(results[t][c][0] for t in regressions[c])
        lines.append(f"VC7.1-passing tests that do not pass on {title}: " +
                     (", ".join(f"{n} {k}" for k, n in kinds.most_common()) or "none") + ".")
    lines.append("")
    OUT_MD.write_text("\n".join(lines), encoding="utf-8", newline="\n")
    rows = ["test\tvc71\tmodern86\tmodern64\tdetail"]
    for t in sorted(results):
        r = results[t]
        if r["modern86"][0] != r["vc71"][0] or r["modern64"][0] != r["vc71"][0]:
            detail = r["modern64"][1] or r["modern86"][1] or r["vc71"][1]
            rows.append("\t".join([t, r["vc71"][0], r["modern86"][0], r["modern64"][0], detail.replace("\t", " ")]))
    OUT_TSV.write_text("\n".join(rows) + "\n", encoding="utf-8", newline="\n")
    (WORK / "results.json").write_text(json.dumps(results, indent=1))
    print("\n".join(lines[8:]))


if __name__ == "__main__":
    main()
