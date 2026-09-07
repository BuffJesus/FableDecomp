#!/usr/bin/env python3
"""Differential probe: does the 13.10.4035 (QFE) backend emit different .text than our
RTM 13.10.3077 for the same source? Compiles under both toolsets and compares the
extracted per-function .text bytes (ignoring symbol names / comdat ordering, which differ
cosmetically in the .obj packaging without any machine-code change).

FINDING (2026-08-16): across 128 functions (120 real landed + 8 synthetic incl. x87
float-fold shapes) the .text is byte-identical. 13.10.4035 codegen == 13.10.3077 codegen;
the QFE hotfix was a non-codegen fix. See docs/pipeline/QFE4035_COMPILER_GATE.md.

Usage:
  python qfe_codegen_diff.py <src.cpp> <leaf>     # one function
  python qfe_codegen_diff.py --sample 120         # N random landed sources
Requires the 4035 toolset installed (see fetch_qfe_toolset.ps1 / $VC71_QFE).
"""
from __future__ import annotations
import argparse, os, re, subprocess, random, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import permuter_score as ps  # noqa: E402

_DS = re.compile(r"^\s*[0-9a-fA-F]+\s+<(.+)>:$")
_DB = re.compile(r"^\s*[0-9a-fA-F]+:\s+((?:[0-9a-fA-F]{2}\s+)+)")


def _textblobs(cl: str, srcpath: Path, work: Path):
    """Multiset of per-function .text byte-strings (name-independent)."""
    obj = work / "o.obj"
    if obj.exists():
        obj.unlink()
    e = dict(os.environ)
    e["PATH"] = f"{Path(cl).parent};{ps.VC/'bin'};{e['PATH']}"
    e["INCLUDE"] = f"{ps.VC/'include'};{ps.ROOT/'rebuild'/'include'}"
    e["LIB"] = str(ps.VC / "lib")
    cp = subprocess.run([cl, "/nologo", "/c", "/W3", "/O2", "/Oy", f"/Fo{obj}", str(srcpath)],
                        capture_output=True, text=True, env=e)
    if cp.returncode != 0 or not obj.exists():
        return None
    pr = subprocess.run([ps.OBJDUMP, "-d", str(obj)], capture_output=True, text=True)
    fns, cur = {}, None
    for line in pr.stdout.splitlines():
        s = _DS.match(line)
        if s:
            cur = s.group(1); fns.setdefault(cur, bytearray()); continue
        d = _DB.match(line)
        if d and cur is not None:
            fns[cur].extend(bytes.fromhex(d.group(1)))
    return tuple(sorted(bytes(v).hex() for v in fns.values()))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src", nargs="?", type=Path)
    ap.add_argument("leaf", nargs="?")
    ap.add_argument("--sample", type=int, default=0)
    ap.add_argument("--seed", type=int, default=7)
    a = ap.parse_args()
    rtm = str(ps.VC / "bin" / "cl.exe")
    qfe, _ = ps.resolve_toolset(True)   # errors loudly if 4035 toolset absent
    qfe = str(qfe)
    work = Path(__file__).parent / "examples" / "qfe" / "_diff"; work.mkdir(parents=True, exist_ok=True)

    if a.sample:
        srcs = list((ps.ROOT / "rebuild" / "src" / "compiled").rglob("*.cpp"))
        random.Random(a.seed).shuffle(srcs); srcs = srcs[:a.sample]
        same = diff = cfail = 0; diffs = []
        for s in srcs:
            x = _textblobs(rtm, s, work); y = _textblobs(qfe, s, work)
            if x is None or y is None: cfail += 1; continue
            if x == y: same += 1
            else: diff += 1; diffs.append(s.name)
        print(f"compiled {same+diff}  .text identical: {same}  DIFFERENT: {diff}  cfail: {cfail}")
        for d in diffs[:30]: print("  DIFF:", d)
        return 0 if diff == 0 else 2

    if not a.src or not a.leaf:
        ap.error("give <src> <leaf> or --sample N")
    x = _textblobs(rtm, a.src, work); y = _textblobs(qfe, a.src, work)
    print("3077 == 4035 .text:", x == y)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
