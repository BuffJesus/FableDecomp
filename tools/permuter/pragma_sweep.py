#!/usr/bin/env python3
"""Sweep in-source `#pragma optimize` settings over LANDED-but-DIFFERING candidates.

Why pragmas only: `build_candidates.ps1` compiles every candidate with one fixed flag set
and has no per-entry flag field, so a win that depends on a command-line flag cannot be
carried into the gate. A `#pragma` lives in the source and therefore survives. Retail's
translation units were not built uniformly, which is exactly what these pragmas emulate.

Reads the comparer's `rebuild/compile-gate/retail-parity.tsv`, takes the DIFFER rows whose
built length already equals retail (same length => a codegen-settings or register-allocation
difference, not a wrong model), and tries each pragma on the landed source.

Usage: python pragma_sweep.py [--same-length-only] [--apply] [--max N] [--addr HEX ...]
  --apply   prepend the winning pragma to the landed source (only on an exact/reloc match)
"""
import csv
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from permuter_score import score_source  # noqa: E402

ROOT = Path(r"D:\Documents\FableTLC")
PARITY = ROOT / "rebuild/compile-gate/retail-parity.tsv"
CATALOG = ROOT / "rebuild/build_candidates.ps1"
SRCROOT = ROOT / "rebuild/src/compiled"

PRAGMAS = ['#pragma optimize("s",on)', '#pragma optimize("t",on)',
           '#pragma optimize("g",on)', '#pragma optimize("g",off)',
           '#pragma optimize("",off)', '#pragma optimize("a",on)',
           '#pragma optimize("y",on)', '#pragma optimize("y",off)']

APPLY = "--apply" in sys.argv
SAME_ONLY = "--same-length-only" in sys.argv
MAX = int(sys.argv[sys.argv.index("--max") + 1]) if "--max" in sys.argv else 0
ONLY = {a.lower() for a in sys.argv[sys.argv.index("--addr") + 1:]} if "--addr" in sys.argv else set()


def catalog_sources():
    text = CATALOG.read_text(encoding="utf-8")
    out = {}
    for block in re.findall(r"\[pscustomobject\]@\{(.*?)\n\s*\}", text, re.S):
        a = re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'", block)
        s = re.search(r"Source\s*=\s*'([^']+)'", block)
        if a and s:
            out[a.group(1).lower()] = s.group(1)
    return out


def main():
    sources = catalog_sources()
    rows = list(csv.DictReader(open(PARITY, encoding="utf-8-sig"), delimiter="\t"))
    targets = []
    for r in rows:
        if r["status"] != "DIFFER":
            continue
        if ONLY and r["address"].lower() not in ONLY:
            continue
        if SAME_ONLY and r["retail_bytes"] != r["object_text_bytes"]:
            continue
        targets.append(r)
    if MAX:
        targets = targets[:MAX]
    print("candidates: %d" % len(targets))
    wins = 0
    for r in targets:
        addr = r["address"].lower()
        rel = sources.get(addr)
        if not rel:
            print("  %s  NO_CATALOG_SOURCE" % addr)
            continue
        cpp = SRCROOT / rel
        if not cpp.exists():
            print("  %s  MISSING %s" % (addr, rel))
            continue
        text = cpp.read_text(encoding="utf-8", errors="ignore")
        best = None
        for p in PRAGMAS:
            if p.split("(")[0] in text and p in text:
                continue                      # already carries exactly this pragma
            try:
                # Never create `_score` beside a landed source. Besides polluting source
                # globs, those objects can outlive the experiment and confuse later audits.
                with tempfile.TemporaryDirectory(prefix="fable-pragma-sweep-") as temporary:
                    got = score_source(cpp, addr, prepend=p, workdir=Path(temporary))
            except Exception as exc:          # a source that needs headers we do not stage
                got = {"score": -1, "status": "ERR:%s" % exc}
            if got.get("status") in ("MATCH", "RELOCATION_MATCH"):
                best = (p, got["status"])
                break
        if best:
            wins += 1
            print("  %s  WIN %-28s %s" % (addr, best[1], best[0]))
            if APPLY:
                cpp.write_text(best[0] + "\n" + text, encoding="utf-8")
        else:
            print("  %s  no pragma helps (retail=%s obj=%s prefix=%s)"
                  % (addr, r["retail_bytes"], r["object_text_bytes"], r["matching_prefix_bytes"]))
    print("wins: %d/%d%s" % (wins, len(targets), " (applied)" if APPLY else " (dry run)"))


if __name__ == "__main__":
    main()
