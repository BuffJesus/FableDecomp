#!/usr/bin/env python3
"""Report the function starts the manifest does NOT know about.

A manifest row spans [addr, next_manifest_addr). When the next real function is missing
from the manifest, the row swallows it -- VC7.1 packs tiny functions with no 0xCC between
them, so there is no padding cue. `rowtrim.trim_body` finds the real end of the named
function by control flow; whatever follows the cut is a *different, unlisted function*.

Those cut points are therefore newly discovered function starts: real reverse-engineering
output that every downstream consumer wants (Ghidra labels, coverage metrics, the crawl's
eligible pool). This walks the whole manifest and emits them.

Output: rebuild/manifest/manifest-gaps.tsv
  host_addr  host_name  host_len_raw  host_len_trimmed  gap_addr  gap_len  gap_bytes

`gap_len` is only the distance to the next manifest row, so a gap region may itself hold
more than one function; it is a lower bound on what is missing, not a function count.

Usage: python manifest_gaps.py [--write]
"""
import csv
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from rowtrim import trim_body

ROOT = Path(r"D:\Documents\FableTLC")
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
OUT = ROOT / "rebuild/manifest/manifest-gaps.tsv"

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


rows = list(csv.DictReader(open(ROOT / "rebuild/manifest/functions.tsv", encoding="utf-8-sig"), delimiter="\t"))
addrs = sorted(int(r["address"], 16) for r in rows if r.get("address"))
nextof = {addrs[i]: addrs[i + 1] for i in range(len(addrs) - 1)}
known = set(addrs)

found = []
for r in rows:
    a = r.get("address")
    if not a:
        continue
    va = int(a, 16)
    o = off(va)
    nx = nextof.get(va)
    if o is None or nx is None:
        continue
    raw = data[o:o + (nx - va)]
    ee = len(raw)
    while ee > 0 and raw[ee - 1] in (0xCC, 0x90):
        ee -= 1
    body = raw[:ee]
    if not body:
        continue
    trimmed, cut = trim_body(body, va)
    if cut is None:
        continue
    # skip the alignment padding that follows the cut
    j = cut
    while j < len(body) and body[j] in (0xCC, 0x90):
        j += 1
    if j >= len(body):
        continue
    gap_va = va + j
    if gap_va in known:
        continue
    found.append((a.lower(), r.get("name") or "", len(body), len(trimmed),
                  "%08x" % gap_va, len(body) - j, body[j:].hex()))

print("manifest rows scanned      : %d" % len(rows))
print("over-captured rows         : %d" % len(found))
print("NEW function starts found  : %d" % len({f[4] for f in found}))
tot = sum(f[5] for f in found)
print("bytes of unlisted code     : %d" % tot)

if "--write" in sys.argv:
    with open(OUT, "w", encoding="utf-8", newline="") as f:
        w = csv.writer(f, delimiter="\t", lineterminator="\n")
        w.writerow(["host_addr", "host_name", "host_len_raw", "host_len_trimmed",
                    "gap_addr", "gap_len", "gap_bytes"])
        for row in sorted(found, key=lambda x: x[4]):
            w.writerow(row)
    print("wrote %s" % OUT)
