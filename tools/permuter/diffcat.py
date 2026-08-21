#!/usr/bin/env python3
"""Triage a landed-but-differing candidate: which byte runs are REAL differences?

A raw retail-vs-object diff overstates the problem, because every relocated operand reads
as all-zero in a fresh .obj. This splits the differing runs into relocation slots (ignore
them -- the comparer masks them) and real runs, and prints each real run with context so
the failure mode is visible at a glance: a register swap (`8b 6f 04` vs `8b 5f 04`), a
value kept in a register vs re-read from the stack (`2b c7` vs `2b 44 24 08`), a different
division idiom, or a genuinely wrong model.

It also skips `??_G`/`??_E` destructor thunks when choosing the function in the object --
the same trap that made the comparer report byte-exact functions as DIFFER.

Usage: python diffcat.py <hexaddr> [<hexaddr> ...]
"""
import csv
import re
import subprocess
import sys
from pathlib import Path

OD = (r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages"
      r"\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe"
      r"\mingw64\bin\objdump.exe")
ROOT = Path(r"D:\Documents\FableTLC")
DS = re.compile(r"^\s*[0-9a-fA-F]+\s+<(.+)>:$")
DB = re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2}\s+)+)")


def retail_bytes(addr):
    for r in csv.DictReader(open(ROOT / "rebuild/oracles/auto-re-candidates.tsv",
                                 encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower() == addr:
            return bytes.fromhex(r["bytes"]), r["name"]
    return None, None


def built_bytes(addr):
    obj = ROOT / "rebuild/build/candidates-Release" / addr[:2] / addr[2:4] / addr / (addr + ".obj")
    out = subprocess.run([OD, "-d", str(obj)], capture_output=True, text=True).stdout
    fns, cur = [], None
    for line in out.splitlines():
        m = DS.match(line)
        if m:
            cur = {"sym": m.group(1), "b": bytearray()}
            fns.append(cur)
            continue
        m = DB.match(line)
        if m and cur is not None:
            cur["b"].extend(bytes.fromhex(m.group(2)))
    fns = [f for f in fns if not f["sym"].startswith(("??_G", "??_E"))] or fns
    sel = max(fns, key=lambda f: len(f["b"]))
    return bytes(sel["b"]), sel["sym"]


for addr in sys.argv[1:]:
    addr = addr.lower()
    retail, name = retail_bytes(addr)
    built, sym = built_bytes(addr)
    print("== %s %s" % (addr, (name or "")[:60]))
    print("   retail=%d built=%d  sym=%s" % (len(retail), len(built), sym[:58]))
    diffs = [i for i in range(min(len(retail), len(built))) if retail[i] != built[i]]
    runs, start, prev = [], None, None
    for i in diffs:
        if start is None:
            start = i
        elif i != prev + 1:
            runs.append((start, prev))
            start = i
        prev = i
    if start is not None:
        runs.append((start, prev))
    zero = sum(1 for a, b in runs if all(built[i] == 0 for i in range(a, b + 1)))
    print("   differing bytes=%d in %d runs (%d runs are all-zero in the object = relocations)"
          % (len(diffs), len(runs), zero))
    for a, b in [r for r in runs if not all(built[i] == 0 for i in range(r[0], r[1] + 1))][:8]:
        lo, hi = max(0, a - 5), min(len(retail), b + 6)
        print("     @0x%03x  retail %s" % (a, retail[lo:hi].hex(" ")))
        print("             built  %s" % built[lo:hi].hex(" "))
