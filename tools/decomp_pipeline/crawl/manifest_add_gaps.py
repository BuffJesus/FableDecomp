#!/usr/bin/env python3
"""Teach the manifest the function starts it was missing.

`manifest_gaps.py` finds code that over-captured rows swallow; `xrefs.py` confirms which of
those addresses are actually *entered* from somewhere in the image. This appends the
confirmed ones to `rebuild/manifest/functions.tsv` as first-class rows.

That is the structural fix, not a workaround:
  * a host row's span [addr, next_manifest_addr) now ENDS at the discovered start, so its
    oracle is the real function body without any trimming, and
  * `verify_and_land` (which rejects candidates lacking an authoritative manifest start)
    will accept them, so the crawl can actually land them.

Only addresses that are BOTH (a) recovered as a function start by the control-flow split and
(b) confirmed entered by a call/jmp/stored-pointer are added. Provenance is recorded in
`agent_source` = "gapscan" and `agent_verdict` = "discovered" so these rows are always
distinguishable from Ghidra-derived ones.

Signature fields are left conservative: `calling_convention=unknown`, `prototype_complete=0`.
The crawl's own filters skip incomplete rows, so a discovered row becomes crawl-eligible only
once someone (or `gap_author.py`) supplies a signature -- nothing is silently guessed.

Usage: python manifest_add_gaps.py [--write] [--max-len N]
"""
import csv
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from rowtrim import trim_body
from xrefs import is_entered

ROOT = Path(r"D:\Documents\FableTLC")
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
MANIFEST = ROOT / "rebuild/manifest/functions.tsv"
GAPS = ROOT / "rebuild/manifest/manifest-gaps.tsv"

MAXLEN = int(sys.argv[sys.argv.index("--max-len") + 1]) if "--max-len" in sys.argv else 0
WRITE = "--write" in sys.argv
csv.field_size_limit(10 ** 9)

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


with open(MANIFEST, encoding="utf-8-sig", newline="") as f:
    reader = csv.DictReader(f, delimiter="\t")
    fields = reader.fieldnames
    rows = list(reader)
known = {int(r["address"], 16) for r in rows if r.get("address")}


def split_region(va, region):
    out = []
    pos = 0
    n = len(region)
    while pos < n:
        while pos < n and region[pos] in (0xCC, 0x90):
            pos += 1
        if pos >= n:
            break
        rest = region[pos:]
        body, cut = trim_body(rest, va + pos)
        if cut is None:
            end = len(rest)
            while end > 0 and rest[end - 1] in (0xCC, 0x90):
                end -= 1
            if end:
                out.append((va + pos, rest[:end]))
            break
        out.append((va + pos, body))
        pos += cut
    return out


found = {}
for r in csv.DictReader(open(GAPS, encoding="utf-8-sig"), delimiter="\t"):
    gva = int(r["gap_addr"], 16)
    o = off(gva)
    if o is None:
        continue
    region = data[o:o + int(r["gap_len"])]
    for fva, body in split_region(gva, region):
        if fva in known or fva in found:
            continue
        if MAXLEN and len(body) > MAXLEN:
            continue
        if not is_entered(fva):
            continue
        found[fva] = len(body)

print("discovered starts (xref-confirmed, not already in manifest): %d" % len(found))
print("bytes of newly attributed code: %d" % sum(found.values()))
if not WRITE:
    print("(dry run; pass --write)")
    sys.exit(0)

blank = {k: "" for k in fields}
for va, ln in sorted(found.items()):
    row = dict(blank)
    row["address"] = "%08x" % va
    row["name"] = "sub_%08x" % va
    row["module"] = "_gapscan"
    row["calling_convention"] = "unknown"
    row["return_type"] = "undefined"
    row["parameter_count"] = "0"
    row["prototype_complete"] = "0"
    row["agent_verdict"] = "discovered"
    row["agent_source"] = "gapscan"
    rows.append(row)

rows.sort(key=lambda r: int(r["address"], 16) if r.get("address") else 0)
tmp = MANIFEST.with_suffix(".tsv.new")
with open(tmp, "w", encoding="utf-8", newline="") as f:
    w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
    w.writeheader()
    w.writerows(rows)
tmp.replace(MANIFEST)
print("manifest rows: %d -> %d" % (len(rows) - len(found), len(rows)))
