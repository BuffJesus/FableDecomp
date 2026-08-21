#!/usr/bin/env python3
"""Rank the retail bodies the shape classifier does NOT yet recognise.

`shape_author.py` drains everything `shapes.classify()` knows; this says what to teach
it next. For every un-landed manifest row it trims the body (rowtrim), asks the
classifier, and histograms the misses by exact byte string -- so the top rows are the
biggest remaining families, i.e. the highest-yield new shape classes.

Usage: python shape_census.py [--max-len N] [--top N] [--gapscan-only] [--show-disasm]
"""
import csv
import re
import struct
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from rowtrim import trim_body
from shapes import classify

ROOT = Path(r"D:\Documents\FableTLC")
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")

MAXLEN = int(sys.argv[sys.argv.index("--max-len") + 1]) if "--max-len" in sys.argv else 22
TOP = int(sys.argv[sys.argv.index("--top") + 1]) if "--top" in sys.argv else 30
GAPONLY = "--gapscan-only" in sys.argv

data = EXE.read_bytes()
e = struct.unpack_from("<I", data, 0x3C)[0]
coff = e + 4
nsec = struct.unpack_from("<H", data, coff + 2)[0]
opt = struct.unpack_from("<H", data, coff + 16)[0]
secbase = coff + 20 + opt
secs = []
for i in range(nsec):
    o = secbase + i * 40
    va, = struct.unpack_from("<I", data, o + 12)
    rp, = struct.unpack_from("<I", data, o + 20)
    rs, = struct.unpack_from("<I", data, o + 16)
    vs, = struct.unpack_from("<I", data, o + 8)
    secs.append((va, max(vs, rs), rp))


def off(va):
    rva = va - 0x400000
    for va0, sz, rp in secs:
        if va0 <= rva < va0 + sz:
            return rp + (rva - va0)


rows = list(csv.DictReader(open(ROOT / "rebuild/manifest/functions.tsv", encoding="utf-8-sig"),
                           delimiter="\t"))
addrs = sorted(int(r["address"], 16) for r in rows if r.get("address"))
nextof = {addrs[i]: addrs[i + 1] for i in range(len(addrs) - 1)}


def body(va):
    o = off(va)
    nx = nextof.get(va)
    if o is None or nx is None:
        return b""
    raw = data[o:o + (nx - va)]
    ee = len(raw)
    while ee > 0 and raw[ee - 1] in (0xCC, 0x90):
        ee -= 1
    return trim_body(raw[:ee], va)[0]


cattext = (ROOT / "rebuild/build_candidates.ps1").read_text(encoding="utf-8")
landed = set(m.group(1).lower() for m in re.finditer(r"Address\s*=\s*'([0-9a-fA-F]{8})'", cattext))

miss, hit, examples = Counter(), 0, {}
todo = 0
for r in rows:
    a = r.get("address")
    if not a:
        continue
    ah = "%08x" % int(a, 16)
    if ah in landed:
        continue
    if GAPONLY and r.get("module") != "_gapscan":
        continue
    todo += 1
    b = body(int(a, 16))
    if not b or len(b) > MAXLEN:
        continue
    if classify(b):
        hit += 1
        continue
    miss[b.hex()] += 1
    examples.setdefault(b.hex(), ah)

print("un-landed rows: %d   classifiable (<=%d bytes): %d   unclassified: %d in %d shapes"
      % (todo, MAXLEN, hit, sum(miss.values()), len(miss)))
print("%5s  %-8s  %s" % ("count", "example", "body"))
for h, c in miss.most_common(TOP):
    print("%5d  %-8s  %s" % (c, examples[h], h))
