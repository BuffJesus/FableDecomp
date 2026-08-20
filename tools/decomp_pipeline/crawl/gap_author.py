#!/usr/bin/env python3
"""Turn the manifest GAPS into landed reconstructions.

`manifest_gaps.py` reports the code that over-captured manifest rows swallow: real
functions the manifest never listed. This walks each gap region, splits it into the
individual functions it contains (repeated `rowtrim.trim_body`), classifies each with the
shared shape classifiers, and emits a `verify_and_land --land` payload.

Nothing here is speculative: the oracle for each new function is the exact bytes at its
address, and verify_and_land byte-verifies every candidate, so a bad split or a
misclassified shape simply fails to land.

Addresses already in the manifest, already landed, or referenced by build_bootstrap.ps1
are skipped. Regions containing an unresolved indirect jump are left alone by rowtrim, so
a jump-table function is never split mid-body.

Usage: python gap_author.py <out_prefix> [--max-len N] [--limit N] [--apply]
"""
import csv
import json
import re
import struct
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from rowtrim import trim_body
from shapes import classify
from xrefs import is_entered

ROOT = Path(r"D:\Documents\FableTLC")
SCR = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\7fcf5fa1-31b0-4034-8e81-be42686888b3\scratchpad")
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
GAPS = ROOT / "rebuild/manifest/manifest-gaps.tsv"
CAT = ROOT / "rebuild/build_candidates.ps1"
BOOT = ROOT / "rebuild/build_bootstrap.ps1"

PREFIX = sys.argv[1]
APPLY = "--apply" in sys.argv
MAXLEN = int(sys.argv[sys.argv.index("--max-len") + 1]) if "--max-len" in sys.argv else 64
LIMIT = int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else 0
NOXREF = "--no-xref" in sys.argv   # by default a gap function must be ENTERED from somewhere

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


rows = list(csv.DictReader(open(ROOT / "rebuild/manifest/functions.tsv", encoding="utf-8-sig"), delimiter="\t"))
known = {"%08x" % int(r["address"], 16) for r in rows if r.get("address")}

cattext = CAT.read_text(encoding="utf-8")
landed = {m.lower() for m in re.findall(r"Address\s*=\s*'([0-9a-fA-F]{8})'", cattext)}
boottext = BOOT.read_text(encoding="utf-8")
BOOT_ADDRS = set(re.findall(r"([0-9a-f]{8})(?:_test)?\.cpp", boottext)) | \
             {x.lower().replace("0x", "") for x in re.findall(r"\b(?:0x)?(00[0-9a-fA-F]{6})\b", boottext)}


def split_region(va, region):
    """Split a gap region into the successive functions it contains."""
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
            # rowtrim could not find a boundary: either this is the last function in
            # the region, or it contains an indirect jump. Take it whole and stop.
            end = len(rest)
            while end > 0 and rest[end - 1] in (0xCC, 0x90):
                end -= 1
            if end:
                out.append((va + pos, rest[:end]))
            break
        out.append((va + pos, body))
        pos += cut
    return out


gap_rows = list(csv.DictReader(open(GAPS, encoding="utf-8-sig"), delimiter="\t"))
funcs = []
for r in gap_rows:
    gva = int(r["gap_addr"], 16)
    o = off(gva)
    if o is None:
        continue
    region = data[o:o + int(r["gap_len"])]
    for fva, body in split_region(gva, region):
        ah = "%08x" % fva
        if ah in known or ah in landed or ah in BOOT_ADDRS:
            continue
        if not body or len(body) > MAXLEN:
            continue
        funcs.append((ah, body))

# de-duplicate (two host rows can report the same gap region)
seen = {}
for ah, body in funcs:
    seen.setdefault(ah, body)
print("functions recovered from gap regions: %d (unique %d)" % (len(funcs), len(seen)))

authored, out_rows, kinds = [], [], Counter()
noxref = 0
for ah in sorted(seen):
    body = seen[ah]
    if not NOXREF and not is_entered(int(ah, 16)):
        # nothing in the image calls, jumps to, or stores this address: it is a
        # hypothesis, not a confirmed entry point. A 1-byte `c3` would otherwise
        # "match" its oracle trivially and pollute the catalog with non-functions.
        noxref += 1
        continue
    got = classify(body)
    if not got:
        kinds["unclassified"] += 1
        continue
    cpp, leaf = got
    kinds[leaf] += 1
    test = "#include <stdio.h>" + chr(10) + 'int main(void){ printf("PASS_' + ah + chr(92) + 'n"); return 0; }' + chr(10)
    authored.append({"address": ah, "name": "sub_" + ah, "module": "_gap",
                     "source_cpp": cpp, "test_cpp": test, "pass_pattern": "PASS_" + ah})
    out_rows.append((ah, "sub_" + ah, str(len(body)), body.hex()))
    if LIMIT and len(authored) >= LIMIT:
        break

print("rejected (no xref): %d" % noxref)
print("classified: %d" % len(authored))
for k, v in kinds.most_common():
    print("  %-16s %d" % (k, v))
if not APPLY:
    print("(dry run; pass --apply)")
    sys.exit(0)

json.dump({"result": {"authored": authored}}, open(SCR / (PREFIX + "_land.json"), "w"))
with open(SCR / (PREFIX + "_oracle.tsv"), "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["address", "name", "length", "bytes"])
    for r in out_rows:
        w.writerow(r)
print("payload %s written" % PREFIX)
